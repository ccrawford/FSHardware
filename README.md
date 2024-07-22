# FSHardware
FlightSim Projects
Various personal projects that implement hardware controls for MSFS 2020.
- G1000 (MFD and PFD)
- GMA (Audio stack for G1000)
- KAP-140 (Autopilot)
- GNS530 (Smaller GPS unit)
- Trim Wheel
- GFC500 Autopilot
- KT75C Transponder

A2A Comanche Specific
- Floor Fuel Valves



Types of devices:
Joysticks/HID: Device emulates a joystick but in a non-traditional enclosure. E.g. slide throttle or analog trim wheel.
Button Boxes: Toggle switches, push buttons, rotary devices with cockpit specific layouts. E.g. Comanche button box, GFC500.
Character displays: 7-segment or multi-LED displays. E.g. Radios.
Popped out video: Uses an HDMI display to show a pop-out instrument from MSFS E.g. G1000, GNS530, KLN 90B GPS.
Custom video display: Completely re-creates the display in the panel on the interface device using input parameters from the sim. E.g. STEC-30 or SAM. 
Hybrid: Many of the devices are really hybrid. E.g. the GNS530 and KLN 90B use "button box" in the frame of a "popped out video" display.

Techniques Used:

Software/Integration: 

Joystick: This technique uses an Arduino compatible microcontroller with a joystick library to create a physical control with a joystick interface back to the PC. Where possilbe I will use this technique as it puts all the integration effort into the sim and is the most portable and stable as it doesn't rely on additional software running on the PC. E.g. the analog trim wheel I built for FSX a decade ago "just worked" when I plugged it into MSFS in 2024. This usually involves creating a peripheral with a lot of buttons, pots, or encoders. Then in MSFS you map these buttons/axis to the sim controls. The problem with this is only a limited set of in-game elements can have assignments this way. Also, it's "input only". Getting data back from the sim to control LEDs or displays is not going to happen. 

Mobiflight-basic: Mobiflight is a swiss-army knife for interfacing hardware to MSFS. Open-source and very active community. You build your control, wire it to a compatible arduino or rp2040 and then define inputs and outputs between the sim and the hardware. The integration interface is a little peculiar (e.g. there are about four completely different ways to modify values between the sim and your hardware) and takes a lot of getting used to especially when you're doing displays. That said, it's in active development with a great lead dev and community. I reach for this for most of my hardware, and you'll see the mobiflight hardware defitions and configuration information in the folders.

Mobflight-custom: In early 2024, Mobiflight added the ability for devs to write their own custom interfaces. This is really nice if you want to do a non-standard display. (E.g. creating a digital turn-indicator and auto-pilot.) This is a fairly complex approach, and is likely to require on-going maintenance to keep working. You need decent C/C++ programming skills using VS Code and PlatformIO. 

Emulation: Early in MSFS 2020 connectivity options were limited. The folks at RealSimGear created a nice interface between their Arudino based hardware and MSFS. By emulating the output of their devices you could easily integrate with MSFS. This required decent reverse engineering of their protocols, but once that work was done the code was pretty straight forward. I used this in some of my older G1000, GNS530, and GFC500 devices. I would not use this again today as there are more acceptable/open alternatives and it's unclear to me if this was ever an acceptable use of RealSimGear's IP.

Hardware:
Cases and fascia:
I mostly 3D print things on my ancient FlashForge Creator Pro after designing them in Fusion360. 
I'm adequate in Fusion360, and I include my designs here as .step and .stl. I use different techniques for lettering. Lately I've been printing in white, spray-painting black, and then sanding off the black surface from raised letters. Other times (espcially for buttons) I will recess the letters and then fill with paint (or crayon) then scrape any excess of the button surface. Some designs I use 2-color printing and print the body in one color and the letters in the contrasting color. 

Electronics:
For simple things I'm only going to build one of (e.g. the Comanche lower-left button box) I use panel mount devices and hard-wire, point to point, the buttons and encoders to the microcontroller. 
For more complex things, especially things that use 7-segment LEDS, increasingly I will design a custom PCB and mount the input and output devices to the PCB, using the PCB as a structural member. 
I've also built stand-alone modules that can then be wired into a more complex (or simple!) device. E.g. two  12 character 7-segment display modules that are then wired to the microcontroller.
I will often build on protoboard cut to size then decide if I want/need something more sturdy or custom. 

I've tried multiple types of wiring: 
- header pins with dupont wires (on the G1000): this is a nice, flexible method, but causes too many problems as dupont wires have a tendency to work loose. 
- Direct soldering buttons to microprocessor: this works well if you have panel-mount devices, a sturdy panel to mount them to, and don't need things like pull-up resistors or filters. Using mult-strand, 24awg silicon wiring works the best for me.
- Breadboard: Strictly for prototyping only. Not compact or durable enough to put into use.
- Custom PCB: The way to go, but long turn-around, high-stakes on getting it right the first time, and kind of wasteful.
- Wirewrap: An old technique that is better than dupont wires, but not nearly as good as soldering.

Supplies:
Finding the right switch is a treasure hunt! Often you can't find exactly what you need mainly because the terminology can be inconsistent. Here are some handy part numbers:
ALPS EC11 series encoders:
    - EC11E09244BS D-Shaft, Knurled-shaft (also called "plum") in different lenths and mounting styles. About $1 each. I like D-Shaft panel mount varieties.
    - EC11EBB24C03  dual-shaft (or "dual axis") rotary encoders. These are about $5-8 each and I use a lot of them!
    - 6x6 momentary tactile switches. These are the little clicky-clicky push buttons. You can get them in different button lenths. I use a lot of these. Super cheap. Get an assortment of button heights.
    - R16-503: These are 16MM lighted pushbuttons (momentary and latching). Handy when you want the indicator light to also be a button, like on the Narco CP 136 audio panel.
    