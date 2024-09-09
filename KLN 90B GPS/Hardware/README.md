Note: I have not built a schematic/PCB for this device as of Sept, 2024. I just wired point-to-point from the switches/encoders to a cheap RP2040.

Here's what you need to build this:

1. Display: I used a "raspberry pi 3.5" HDMI screen" These are usually pretty cheap: about $25-30 or so. Available on amazon, aliexpress, etc.
2. RP2040 Pico: I get the long green ones. They're about $2.50 on AliExpress, or $6 on Amazon. I go through a lot, so I buy about 10 at a time from AliExpress.
3. Dual Rotary Encoders with pushbutton: I get these from AliExpress. I'm trying EC110701N2B lately as they are more redily available ($4-5 each), but EC11EBB24C03 are the benchmark ones. They are about $7-8 each on Aliexpress. 
4. One rotary encoder, your pick. EC11E15204a3 works. 
5. Tactile push buttons: 6x6x9  (base is 6x6 mm, then get an appropriate hight. I think I used 9mm, but I have a lot of different sizes so not positive)
6. Perf board: some 0.254mm perf board. I used one of those larger brown ones.
7. Hookup wire: I like 24 guage silicon wire. I strip some solid core 24 guage to use as a common ground.
8. Various size self-tapping screws. Again, whatever on hand. Probably M2s.

Print the two-part fascia. One side has the lettering, the other has the mounting stand-offs. 
Use the fascia to determine where to put the switches and where to cut out the perf board to accomodate the screen. 

Optionally print the GPS Mount. It goes with a similar cleat that I have on the top of my Honeycomb Alpha. I'm 90% sure it's "Honeycomb Top (6)" that i've included here.

Put the switches at appropriate places on the perf board. Wire up the switches to ground and the RP2040. Done.

