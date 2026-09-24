#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <GxEPD2_BW.h>

#define GxEPD2_DRIVER_CLASS GxEPD2_750_GDEY075T7

#define EINK_CS     6
#define EINK_DC     7
#define EINK_RST    8
#define EINK_BUSY   9
#define SD_CS       10
#define SPI_MOSI    11
#define SPI_SCK     12
#define SPI_MISO    13
#define EINK_PWR_EN 14

GxEPD2_BW<GxEPD2_DRIVER_CLASS, GxEPD2_DRIVER_CLASS::HEIGHT> display(
  GxEPD2_DRIVER_CLASS(EINK_CS, EINK_DC, EINK_RST, EINK_BUSY));

const char *TRAIL_BMP     = "/trail.bmp";
const float FIXED_HEADING = 45.0;
const int SCALE = 3;

void drawBMP(const char *filename);
void drawCompass(int16_t cx, int16_t cy, int16_t r, float headingDeg);
uint16_t read16(File &f);
uint32_t read32(File &f);

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(EINK_PWR_EN, OUTPUT);
  digitalWrite(EINK_PWR_EN, LOW);
  delay(100);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);

  pinMode(EINK_CS, OUTPUT);
  digitalWrite(EINK_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  if (!SD.begin(SD_CS, SPI, 4000000)) {
    Serial.println("SD Card Mount Failed!");
  }

  display.init(115200, true, 2, false);
  display.setRotation(1);

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    drawBMP(TRAIL_BMP);
    drawCompass(display.width() - 40, 40, 25, FIXED_HEADING);
  } while (display.nextPage());

  Serial.println("Demo screen drawn.");
}

void loop() {
}

void drawCompass(int16_t cx, int16_t cy, int16_t r, float headingDeg) {
  display.drawCircle(cx, cy, r, GxEPD_BLACK);

  float rad  = (headingDeg - 90.0) * (PI / 180.0);
  int16_t ex = cx + (int16_t)(cos(rad) * r);
  int16_t ey = cy + (int16_t)(sin(rad) * r);

  display.drawLine(cx, cy, ex, ey, GxEPD_BLACK);
  display.fillCircle(ex, ey, 3, GxEPD_BLACK);

  display.setCursor(cx - 3, cy - r - 10);
  display.print("N");
}

uint16_t read16(File &f) {
  uint16_t v;
  ((uint8_t *)&v)[0] = f.read();
  ((uint8_t *)&v)[1] = f.read();
  return v;
}

uint32_t read32(File &f) {
  uint32_t v;
  ((uint8_t *)&v)[0] = f.read();
  ((uint8_t *)&v)[1] = f.read();
  ((uint8_t *)&v)[2] = f.read();
  ((uint8_t *)&v)[3] = f.read();
  return v;
}

void drawBMP(const char *filename) {
  File bmpFile = SD.open(filename, FILE_READ);
  if (!bmpFile) {
    Serial.print("File not found: ");
    Serial.println(filename);
    display.setCursor(5, 15);
    display.print("BMP NOT FOUND");
    return;
  }

  if (read16(bmpFile) != 0x4D42) {
    Serial.println("Not a valid BMP file");
    display.setCursor(5, 15);
    display.print("NOT A BMP FILE");
    bmpFile.close();
    return;
  }

  read32(bmpFile);
  read32(bmpFile);
  uint32_t dataOffset = read32(bmpFile);
  uint32_t headerSize = read32(bmpFile);
  int32_t bmpWidth   = (int32_t)read32(bmpFile);
  int32_t bmpHeight  = (int32_t)read32(bmpFile);
  read16(bmpFile);
  uint16_t bitDepth  = read16(bmpFile);
  uint32_t compression = read32(bmpFile);

  Serial.print("BMP: "); Serial.print(bmpWidth); Serial.print("x"); Serial.print(bmpHeight);
  Serial.print(", "); Serial.print(bitDepth); Serial.println("-bit");

  if (compression != 0) {
    Serial.println("Compressed BMPs are not supported - re-export as uncompressed.");
    display.setCursor(5, 15);
    display.print("BMP IS COMPRESSED");
    bmpFile.close();
    return;
  }
  if (bitDepth != 1 && bitDepth != 4 && bitDepth != 8 && bitDepth != 24) {
    Serial.print("Unsupported BMP bit depth: ");
    Serial.println(bitDepth);
    display.setCursor(5, 15);
    display.print("UNSUPPORTED: ");
    display.print(bitDepth);
    display.print("-BIT BMP");
    bmpFile.close();
    return;
  }

  bool paletteBlack[256];
  if (bitDepth <= 8) {
    uint32_t paletteStart = 14 + headerSize;
    uint32_t paletteBytes = dataOffset - paletteStart;
    uint32_t numColors = paletteBytes / 4;
    if (numColors > 256) numColors = 256;

    bmpFile.seek(paletteStart);
    for (uint32_t i = 0; i < numColors; i++) {
      uint8_t b = bmpFile.read();
      uint8_t g = bmpFile.read();
      uint8_t r = bmpFile.read();
      bmpFile.read();
      paletteBlack[i] = ((r + g + b) / 3) < 128;
    }
  }

  bool topDown = false;
  if (bmpHeight < 0) {
    bmpHeight = -bmpHeight;
    topDown = true;
  }

  int32_t outW = bmpHeight * SCALE;  
  int32_t outH = bmpWidth  * SCALE;

  int16_t x = (display.width()  - outW) / 2;
  int16_t y = (display.height() - outH) / 2;
  if (x < 0) x = 0;
  if (y < 0) y = 0;

  uint32_t rowSize = ((bmpWidth * bitDepth + 31) / 32) * 4;
  uint8_t *rowBuf = (uint8_t *)malloc(rowSize);
  if (!rowBuf) {
    Serial.println("Not enough RAM to read this BMP row.");
    display.setCursor(5, 15);
    display.print("OUT OF MEMORY");
    bmpFile.close();
    return;
  }

  for (int32_t row = 0; row < bmpHeight; row++) {
    int32_t srcRow = topDown ? row : (bmpHeight - 1 - row);
    bmpFile.seek(dataOffset + (uint32_t)srcRow * rowSize);
    bmpFile.read(rowBuf, rowSize);

    for (int32_t col = 0; col < bmpWidth; col++) {
      bool black;

      if (bitDepth == 1) {
        uint8_t byteVal = rowBuf[col / 8];
        uint8_t idx = (byteVal >> (7 - (col % 8))) & 0x01;
        black = paletteBlack[idx];
      } else if (bitDepth == 4) {
        uint8_t byteVal = rowBuf[col / 2];
        uint8_t idx = (col % 2 == 0) ? (byteVal >> 4) : (byteVal & 0x0F);
        black = paletteBlack[idx];
      } else if (bitDepth == 8) {
        black = paletteBlack[rowBuf[col]];
      } else {
        uint8_t b = rowBuf[col * 3 + 0];
        uint8_t g = rowBuf[col * 3 + 1];
        uint8_t r = rowBuf[col * 3 + 2];
        black = ((r + g + b) / 3) < 128;
      }

      if (black) {
        int16_t dx = x + row * SCALE;
        int16_t dy = y + (bmpWidth - 1 - col) * SCALE;
        display.fillRect(dx, dy, SCALE, SCALE, GxEPD_BLACK);
      }
    }
  }

  free(rowBuf);
  bmpFile.close();
}