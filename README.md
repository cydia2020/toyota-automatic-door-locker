# toyota-automatic-door-locker
Arduino based automatic door locker for Toyota Prius, α and Prime

For testing purposes only.

Needs MCP2515 and any arduino compatible with Seeed's CAN Bus lib.

Goal:
1. Unlock all doors when disengaging parking brake.
2. Lock all doors when engaging parking brake.

Known issues:
1. The car sometimes ignore whatever we send, we should have some sort of detection for door lock status.

#### Sending untested CAN commands to your BCM while the vehicle is turned ON is a BAD IDEA, and could damage your vehicle, leading to expensive ECU swaps and repairs. Please do not try this if you don't know what you are doing.
#### These commands are only tested on the Toyota Prius (ZVW50/51/52), Prius v (a.k.a Prius alpha)(ZVW40/41), and Corolla (E210). USE AT YOUR OWN RISK!
#### Different cars will have different BCM signals, this code doing what it does on your car DOES NOT MEAN it is the appropriate code for you car. Please use a CAN Packet sniffer to find and adapt the correct signal before sending it to your vehicle.
# I AM NOT RESPONSIBLE FOR ANY DAMAGES.
