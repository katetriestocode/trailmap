# 6/30/2026 16 PM - cost reduction!

_Time spent: 1h_

I got the cost down! assembly of only two boards, no standard pieces, and not assembling everything takes it down to around 80$! I also changed the magnetometer to a LIS2MDL, instead of a lis3mdl which i found out is out of production! now i will look at the other pieces i will have to hand solder, as i am unsure how i will solder smd pieces like the esp
<img width="768" height="408" alt="{F266392B-B6CA-4F24-A104-2809517627C4}" src="https://github.com/user-attachments/assets/4cca1306-0203-4d59-9302-bc4e76500566" />
<img width="1486" height="769" alt="image" src="https://github.com/user-attachments/assets/0b879089-aaaa-428c-87a3-af4316f233a1" />



# 6/28/2026 10 PM - more wiring + first price check!

_Time spent: 1h_

i started wiring other components but the important ones, fixing errors, and establishing a better layout. I checked prices for just boards, and found everything under 100x100mm is so much cheaper than anything slightly above, so i reduced the space to 100mm horizontally and vertically 100mm, which i can easily reduce based on the screen size i will decided. I got a first pcba price estimate from jlcpcb, and it was 250$??? this is cuz theres like more than 100 dollars of various fees because it is standard rather than econ (constraint based on needing the esp32 wroom 1, which isnt in econ :P) i will drastically reduce the price by soldering it myself (maybe with a hot plate?)
(https://lapse.hackclub.com/timelapse/qy0DC-9HV17Z)
<img width="993" height="634" alt="Screenshot 2026-06-29 at 22 47 47" src="https://github.com/user-attachments/assets/5c5f8d04-37c3-435e-b4a2-b1d2ce8057ae" />
<img width="1277" height="688" alt="Screenshot 2026-06-29 at 22 46 58" src="https://github.com/user-attachments/assets/28c25db9-f35c-48df-b809-537bd7797f5f" />



# 6/27/2026 7 PM - wiring start!

_Time spent: 2h_

i found out that i need a HAT to connect the e-ink to my board, which is about 8 euros on aliexpress. I then started wiring, being careful to place decouplers near their adjacent part, and wiring the important wires first (eg info parts and battery)
(https://lapse.hackclub.com/timelapse/zSaDSLh_QurV)
<img width="752" height="612" alt="Screenshot 2026-06-29 at 22 40 21" src="https://github.com/user-attachments/assets/d822834e-9c49-4fdf-9e10-1834d4b81394" />



# 6/27/2026 1 PM - switches, buttons, e-ink, and layout start!

_Time spent: 2h_

i struggled choosing which buttons to use, but ended up choosing one with an attachment that would allow a custom cap to make the buttons look more professional. I also started laying out the items based on the e-ink layout, considering that the gps should be away from everything else, and that the usb-c and sd card need to be in a convenient position.
(https://lapse.hackclub.com/timelapse/XH70X7rhDU6x)+(https://lapse.hackclub.com/timelapse/bGLBh-pUC-5d)
<img width="587" height="435" alt="Screenshot 2026-06-29 at 22 36 18" src="https://github.com/user-attachments/assets/73536c22-a7a4-4160-85fe-229b2f3944cd" />



# 6/26/2026 5 PM - footprint assignment!

_Time spent: 2h_

assigned footprints! i assigned all the various footprints (except buttons) i struggled figuring out what components corresponded to which SOTs :D
(https://lapse.hackclub.com/timelapse/Y9WYruspzZDs)
<img width="811" height="598" alt="Screenshot 2026-06-29 at 22 28 19" src="https://github.com/user-attachments/assets/580d377e-3c07-44c9-a967-964916f7f334" />



# 6/26/2026 10 AM - finished schematic!

_Time spent: 3h_

completely finished wiring, with 0 erros now! took ages to find all the needed decoupling and to figure out were pullups were needed, since i am still a beginenr haha. really proud of it so far. i also struggled with pwr_flags since i kept getting erros about them. im gonna start placing the components next!
![Screenshot 2026-06-26 at 12.37.10](https://stasis.hackclub-assets.com/images/1782470270406-mc7r0i.png)

![image](https://stasis.hackclub-assets.com/images/1782470270406-mc7r0i.png)

# 5/21/2026 4 PM - more wiring

_Time spent: 0.5h_

wired the gps module! it was really tricky as i could not find a lot of schematics online, i got it mostly done but had to ask AI how to wire vcc_rf:

![image](https://stasis.hackclub-assets.com/images/1779379597670-xiz8p8.png)

i also started wiring the e-ink, and used the ao3407a documentation to wire it :P

![image](https://stasis.hackclub-assets.com/images/1779379672601-dmffce.png)

![image](https://stasis.hackclub-assets.com/images/1779379597670-xiz8p8.png)
![image](https://stasis.hackclub-assets.com/images/1779379672601-dmffce.png)

# 5/21/2026 8 AM - wiring the battery protection

_Time spent: 1h_

i wired the battery protection, which turned out to be a huge hassle. i was struggling with the documentation until i found this diagram online:

![image](https://stasis.hackclub-assets.com/images/1779353462892-zw6ng3.png)

which really helped me wire everything. tho, my 1 pin is floating (together with the 8 since theyre together), and i asked ai what to do, so apparently i should leave a copper 'island' for it in the pcb to act as a heatsink. ill double check this later.

this is what i did, yay!
![image](https://stasis.hackclub-assets.com/images/1779353428693-2ozmke.png)

i also started scheming the usb:

![image](https://stasis.hackclub-assets.com/images/1779353577769-s6l0ek.png)

which i also wired the protection, plus the boot and reset switches for the esp :PP

![image](https://stasis.hackclub-assets.com/images/1779353462892-zw6ng3.png)
![image](https://stasis.hackclub-assets.com/images/1779353428693-2ozmke.png)
![image](https://stasis.hackclub-assets.com/images/1779353577769-s6l0ek.png)

# 5/19/2026 4 PM - found and placed components!

_Time spent: 1h_

so i found the missing symbols and footprints, and loaded everything into kicad. i mapped everything out according to its role:
![image](https://stasis.hackclub-assets.com/images/1779209108422-t69cvx.png)
so it (atm) looks like this:

![image](https://stasis.hackclub-assets.com/images/1779209213664-azowzx.png)

![image](https://stasis.hackclub-assets.com/images/1779209108422-t69cvx.png)
![image](https://stasis.hackclub-assets.com/images/1779209213664-azowzx.png)

# 5/12/2026 7 PM - planning and part finding!

_Time spent: 1.5h_

I started by roughly finding all the parts i wanted for the functions!
The functions i plan are:

- [ ] e-ink display
- [ ] live updating map
- [ ] preloaded trail route map
- [ ] live positioning
- [ ] long lasting battery
- [ ] usb-c charging
- [ ] compass
- [ ] altimeter
- [ ] direction pointing
- [ ] clock
- [ ] temperature
- [ ] sensor

Additionally, since this is made for outdoors, i need it to be safe, reliable and durable, so i added all the possible precautions regarding power.

![Screenshot 2026-05-12 at 20.58.02](https://stasis.hackclub-assets.com/images/1778615885667-ccnw1u.png)

i also started reflecting on the actual screen, but i found that e-ink displays can get really expensive really fast. I provisionally decided on a 5.8" wave-share display, but i need to check regarding partial refresh, as in rapid movements it would be beneficial for it to not repeatedly refresh the whole page.

![image](https://stasis.hackclub-assets.com/images/1778615885667-ccnw1u.png)
