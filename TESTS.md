The tests branch of gca-node are initial tests that prove that communication with the Wii U GameCube&trade; Adapter is possible on most computer machines that support NodeJS.
These tests also provide tools for polling data read from the GameCube.

## LibUSB tests
LibUSB tests provide testing of status of communication on Visual C++ with LibUSB.

### Building

## NodeBridge tests
As the name suggests, these tests provide the bridge interface between NodeJS and the Wii U&trade; GameCube Adapter.

NodeBridge 1 test sends a test interrupt transfer to start communicating with the adapter.
NodeBridge 2 test sends an interrupt transfer to read raw data from the adapter.

### Building
Building NodeBridge tests work pretty much like building addons for Nodejs. On your command prompt, type in:
 1. `npm install node-gyp -g` to install node-gyp to your PATH.
 2. `npm install nan --save` to install NAN, a supplementary utility to build the tests.
 3. `node-gyp rebuild` to configure the source code and build the addon from it.

### Raw data info
The raw data obtained from the Adapter in NodeBridge Test 3 should be an array of chars of length 37. The structure should be the following:
`[Header] [Status] [Buttons1] [Buttons2] [Left stick X] [Left stick Y] [C stick X] [C stick Y] [L axis] [R axis]`
Except for the first element, all of these elements appear four times in the array, one for each port of the adapter, thus one for each controller. 
(4 controllers * 9 chars + 1 header = 37 elements)

 * [Header] seems to be a header char for the entire adapter itself.
 * [Status] seems to be a boolean for checking if a controller is connected on the designed port.
 * [Buttons1] contains information about the status of 8 buttons, determined by bits written from left to right:
    * Bit 1: D-Pad UP button
    * Bit 2: D-Pad DOWN button
    * Bit 3: D-Pad RIGHT button
    * Bit 4: D-Pad LEFT button
    * Bit 5: Y button
    * Bit 6: X button
    * Bit 7: B button
    * Bit 8: A button
 * [Buttons2] contains information about the status of the remaining 4 buttons, determined by the least significant nibble, from left to right:
    * Bit 5: L trigger button
    * Bit 6: R trigger button
    * Bit 7: Z button
    * Bit 8: START button
* [Left stick X] represents the horizontal axis of the left stick. It could actually be a short or even a float.
* [Left stick Y] represents the vertical axis of the left stick. It could actually be a short or even a float.
* [C stick X] represents the horizontal axis of the C-Stick. It could actually be a short or even a float.
* [C stick Y] represents the vertical axis of the C-Stick. It could actually be a short or even a float.
* [L axis] represents the analog input axis of the L trigger. It could be a short.
* [R axis] represents the analog input axis of the R trigger. It could be a short.

Rumble data is not obtained from here. Rather, it seems to be a command data.