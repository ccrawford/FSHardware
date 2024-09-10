Physical controls, display, and mounting for [falcon71's KLN 90B](https://github.com/falcon71/kln90b) for MSFS.

In June-ish of 2024 A2A added support for the open-source KLN 90B to the Comanche. Although my current radio stack is full, I wanted to add this somehow.

The basic construction of this is "a button box connected by Mobiflight with a small HDMI screen." It's very similar to my G1000 or GNS530.

Total material cost is probably around $60. Major expense: a 3.5" Raspberry Pi HDMI screen for about $35, two dual-axis rotary encoders (2 @ $7 each), a RP2040 Pico ($3), and assorted tactile switches, wire, 3D printer filament, proto-board. ($3)

Hardware:
See the [Hardware Readme](https://github.com/ccrawford/FSHardware/blob/main/KLN%2090B%20GPS/Hardware/README.md) for parts and assembly.

Software:
- Uses a basic Mobiflight setup. No custom devices or software.
- Mobiflight config files in the [Software Folder](https://github.com/ccrawford/FSHardware/tree/main/KLN%2090B%20GPS/Software)

Build steps:
- See the Hardware section. 

Notes:
This is still an "alpha" version, but I've been using it weekly during "GA Tuesday" flights with ThePilotClub.org (a great group of VatSim-flying Flight Sim nerds.) It's not terribly durable in its current configuration, but it is completely functional. If there's interest, I may make a custom PCB to improve the strength and durability and size of the unit. But really, since I only need one, and this one-off is built, it's not at the top of the project list.

Photos:
Basic front view
![View of KLN 90B](https://github.com/ccrawford/FSHardware/blob/main/KLN%2090B%20GPS/Photos/KLN%2090B%20800x320.jpg)

Sideview showing buttons, mounting
![Side view of KLN 90B](https://github.com/ccrawford/FSHardware/blob/main/KLN%2090B%20GPS/Photos/KLN%2090B%20side%20view.jpg)

Rearview showing ugly-ass point-to-point wiring
![Rear view of KLN 90B](https://github.com/ccrawford/FSHardware/blob/main/KLN%2090B%20GPS/Photos/KLN%2090B%20point%20to%20point.jpg)

Not shown:
Mount to allow easy attach/detach from Honeycomb Alpha yoke

TODO:
As of Sep, 2024, none of these are in progress. It works great as is, although I'm a little concerned about durability.

- Build a better surrounding case to better enclose wiring and support external connectors
- Build the secondary button/display cluster for the panel. I use this so infrequently though, and I don't have a good mounting place for my setup.
- Create a custom PCB to improve rigidity and durability
- Incorporate into my existing Comanche radio stack
- Source a more appropriate display (reduced height)
