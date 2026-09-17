#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_BMP390.h>
#include <Adafruit_SHT4x.h>
#include <GxEPD2_BW.h> 


#define I2C_SDA 4
#define I2C_SCL 5
#define GPS_TX 37
#define GPS_RX 36
#define SD_CS 18
#define EINK_CS 6
#define EINK_DC 7
#define EINK_RST 12
#define EINK_BUSY 17


#define BTN_ZOOM_IN 31
#define BTN_ZOOM_OUT 32
#define BTN_A 47
#define BTN_B 48

#define BTN_UP 41
#define BTN_DOWN 38
#define BTN_LEFT 42
#define BTN_RIGHT 21

HardwareSerial GPSSerial(1);
TinyGPSPlus gps;

Adafruit_LIS3MDL magnetometer;
Adafruit_BMP390 altimeter;
Adafruit_SHT4x sht40;

GxEPD2_BW<x::HEIGHT> display(x(EINK_CS, EINK_DC, EINK_RST, EINK_BUSY));


int currentZoom = 14; 
const int MIN_ZOOM = 10;
const int MAX_ZOOM = 16;


double currentLat = 45.4384;
double currentLon = 10.9916;
float heading = 0.0;
float currentTemp = 0.0;
float currentAlt = 0.0;

bool mapNeedsRefresh = true;


void updateSensors();
void handleButtons();
void drawMapScreen();
void drawBMPFromSD(const char *filename, int16_t x, int16_t y);
void drawCompass(int16_t x, int16_t y, float head);
int long2tilex(double lon, int z);
int lat2tiley(double lat, int z);


void setup() {
  Serial.begin(115200);
  

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!magnetometer.begin_I2C()) Serial.println("LIS3MDL Init Failed!");
  if (!altimeter.begin_I2C()) Serial.println("BMP390 Init Failed!");
  if (!sht40.begin()) Serial.println("SHT40 Init Failed!");


  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);


  if (!SD.begin(SD_CS)) Serial.println("SD Card Mount Failed!");


  display.init(115200, true, 2, false); // Init with diagnostic output
  display.setRotation(1); // Landscape
  display.fillScreen(GxEPD_WHITE);
  display.display();


  pinMode(BTN_ZOOM_IN, INPUT_PULLUP);
  pinMode(BTN_ZOOM_OUT, INPUT_PULLUP);
  pinMode(BTN_A, INPUT_PULLUP);
}


void loop() {
  while (GPSSerial.available() > 0) {
    if (gps.encode(GPSSerial.read())) {
      if (gps.location.isUpdated()) {
        currentLat = gps.location.lat();
        currentLon = gps.location.lng();
      }
    }
  }


  updateSensors();


  handleButtons();


  if (mapNeedsRefresh) {
    drawMapScreen();
    mapNeedsRefresh = false;
  }
  
  delay(50);
}


void updateSensors() {
  sensors_event_t event;
  magnetometer.getEvent(&event);
  

  heading = atan2(event.magnetic.y, event.magnetic.x) * 180.0 / M_PI;
  if (heading < 0) heading += 360.0;

  sensors_event_t humidity, temp;
  sht40.getEvent(&humidity, &temp);
  currentTemp = temp.temperature;

  currentAlt = altimeter.readAltitude(1013.25); 
}

void handleButtons() {

  if (digitalRead(BTN_ZOOM_IN) == LOW) {
    if (currentZoom < MAX_ZOOM) {
      currentZoom++;
      mapNeedsRefresh = true;
    }
    delay(300);
  }
  
  if (digitalRead(BTN_ZOOM_OUT) == LOW) {
    if (currentZoom > MIN_ZOOM) {
      currentZoom--;
      mapNeedsRefresh = true;
    }
    delay(300);
  }

  if (digitalRead(BTN_A) == LOW) {
    mapNeedsRefresh = true;
    delay(300);
  }
}


void drawMapScreen() {

  int tileX = long2tilex(currentLon, currentZoom);
  int tileY = lat2tiley(currentLat, currentZoom);


  String filePath = "/" + String(currentZoom) + "/" + String(tileX) + "/" + String(tileY) + ".bmp";
  
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    

    drawBMPFromSD(filePath.c_str(), 0, (display.height() - 256) / 2); 


    int uiX = 200;
    display.fillRect(uiX, 0, display.width() - uiX, display.height(), GxEPD_WHITE);
    display.drawRect(uiX, 0, display.width() - uiX, display.height(), GxEPD_BLACK);


    display.setTextColor(GxEPD_BLACK);
    display.setCursor(uiX + 5, 10);
    display.print("Zoom: "); display.print(currentZoom);
    
    display.setCursor(uiX + 5, 30);
    display.print("Alt: "); display.print(currentAlt, 0); display.print("m");
    
    display.setCursor(uiX + 5, 50);
    display.print("Temp: "); display.print(currentTemp, 1); display.print("C");

    // 6. Draw Compass User Indicator
    drawCompass(uiX + 45, 100, heading);

  } while (display.nextPage());
}

void drawCompass(int16_t x, int16_t y, float head) {
  // Draw outer ring
  display.drawCircle(x, y, 15, GxEPD_BLACK);
  

  float rad = (head - 90.0) * (M_PI / 180.0);
  int16_t endX = x + (cos(rad) * 15);
  int16_t endY = y + (sin(rad) * 15);
  
  display.drawLine(x, y, endX, endY, GxEPD_BLACK);
  display.fillCircle(endX, endY, 3, GxEPD_BLACK); // Arrow tip
}


int long2tilex(double lon, int z) { 
  return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z))); 
}

int lat2tiley(double lat, int z) { 
  return (int)(floor((1.0 - asinh(tan(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z))); 
}


void drawBMPFromSD(const char *filename, int16_t x, int16_t y) {

  
  File bmpFile = SD.open(filename, FILE_READ);
  if (!bmpFile) {
    Serial.print("File not found: "); Serial.println(filename);
    display.setCursor(x + 10, y + 10);
    display.print("MAP TILE NOT FOUND");
    display.print(filename);
    return;
  }

  bmpFile.close();
}
