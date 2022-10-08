# valve demo
The code in the "engine_valves" folder runs on the Arduino directly conncted to the valves.
The code in the "mission_control" folder runs on the Arduino that is connected to buttons.
Both Arduinos talk to each other using XBee radios connected to their serial ports.

The code is currently broken, but can be used as a reference for building remote engine controllers.

XCTU software and a USB to serial converter is needed to configure the radio settings, or for dignosing radio connection errors.
XCTU software can be downloaded here: https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu
