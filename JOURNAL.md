


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
