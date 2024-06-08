A simple rotary control to control the fuel valves on the A2A Comanche in MSFS 2020
<img src="Photos\IMG_1518.jpg" width="500">
<img src="Photos\IMG_1519.jpg" width="500">

Hardware:
- Two 10k rotary poteniometers. Somthing like a WH148 10k linear taper pot. Ideally one with 180 degree rotation.
- One Arduino Pro Micro (or piggy-back on another one that has two analog inputs open).

Software:
- Uses a basic Mobiflight setup. No custom devices or software.

Other software/tools required:
- Fusion360 to modify the case if needed.
- Mobiflight for the 
- Slicer (I use Simplify3D)
- 3D Printer (I use a FlashForge Creator Pro) and I printed in PLA.

Build steps:
- Print the parts: the case base in black and the fascia in white. Knobs in black.
- Spray paint the fascia black. 
- Sand off the black paint to reveal white lettering on fascia.
- Attach the potentiometers to the fascia, adjusting hole size if needed and twisting the pots to properly align on/off positions of the valves. Note that they will turn different directions for on and off. 
- The fascia does not have a detent for the alignment pin on the pot. Instead: tighten the nut on the pot fairly tight then heat up the alignment pin with your soldering iron. This melts the PLA and you have a perfect alignment slot.
- Solder up the pots to the Arduino pro micro. Of on the three pins of the pot:
    - Middle of left pot goes to A0
    - Middle of right pot goes to A1
    - One outside pin on each goes to ground on Arduino
    - Other outside pin on each goes to VCC on Arduino
    - See [this photo](ComancheFuel/Photos/IMG_1519.jpg) full size for help.
- In Mobiflight configure the Arduino in the MobiFlight Module section with the .mfmc file in the software folder.
- In Mobflight open the .mcc file in the software folder.
- Test!



Notes:
- Since this device has no output, you could also program it as a joystick with two axis. 
- The 3D model should include either some detents, some friction control and a hard-stop pin if your pot has more than a 180 degree rotation.
- The Mobiflight files are specific to the A2A Comanche. Of course you can adapt as needed in Mobiflight for other planes.
- The output configs in Mobiflight are just there for informational/tuning purposes. The "Input configs" are the ones that count.
- The case does not have a bottom plate/floor. It probably should. It would be trivial to add one in Fusion360.
- The fascia is friction-fit into the base. Differences in printers may mean yours needs to be adjusted/trimmed/glued.