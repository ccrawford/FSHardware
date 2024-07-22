I've done multiple trimwheel variations over the years. The first and final ones included here.

The First one:

Stupid simple analog trimwheel included here for posterity.

This was one of the first mechanical devices I created. Uses a Digispark ATtny85 microcontroller with a simple 10k potentiometer.
The pot acts as the axle for the trim wheel. The wheel and the case are 3d printed. 

Once the device is programmed, your computer should recognize it as a joystick. You can then assign the joystick axis as the trim axis in the sim. NOTE: The reason I don't do this any more is that many airplanes will have auto-trim as part of the autopilot. The sim will change your trim and then when you touch the mechanical trim wheel the trim can instantly jump to a new setting which really messes you up. So, while it's a great feeling trim-wheel, it's not recommended unless you only fly very simple planes with no motorized trim.

The housing and mounting design are so amateurish I'm not including them here as there are better available elsewhere.
The case looks roughtly like https://www.sportys.com/trimwheel-pro.html 


The New, Digital version:
I based the hardware design on https://www.thingiverse.com/thing:4290141 which is from the now defunct FlightSimMaker community. I re-built tthe faceplate in Fusion and kept much of the design around mounting the trimwheel and rotary encoder. I replaced the mechanical, servo-based indicator mechanism with LEDs. (The incessent seeking/buzzing of the servo was irritating, and I found it was worse than useless as mine was too easily mis-aligned and would give poor or inacccurate/misleading indication of trim position.)

I created a small, custom PCB based on [LM3914M](https://www.ti.com/lit/ds/symlink/lm3914.pdf) display driver chips and two 10-segmenet LED bar graphs. [Example](https://www.sparkfun.com/products/9935). 
The 20 bar graph can actually indicate 39+ different positions as two bars light for "in between" values. I find this completely sufficient. I use MobiFlight to drive the LEDs and change the encoder pulses to sim trim movement.

The PCB design files (EasyEDAPro Online version) are available in the Hardware folder. The Gerber files are in there as well. You can upload those to JLCPCB and have them make you some. $2 for 5 boards.
NOTE: when you assemble the PCBs you must solder the LEDs first, then the LM3914 DIPs. You will have to solder the DIPs from the top legs as I have two through-hole parts overlapping for space purposes. I probably should have done the 3914s as SMDs, but I had some through-hole on hand already. I've had 0 issues making this solder-from-top technique work.

The PCB mounts with some self-tapping M2.5 screws. You can print a case back if you 

See https://www.thingiverse.com/thing:4290141/ for assembly instructions. As mentioned, this is a fork of that excellent project. You can use the Bushing part, or use a standard RSB bearing. 