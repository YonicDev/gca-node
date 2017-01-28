gca-node is a NodeJS addon that adds Nintendo&trade; Wii U GameCube&trade; Adapter native support for NodeJS applications.

Currently, gca-node is under testing and the final code development has not started.

## Build guide
**Currently, gca-node's initial tests only build on Windows 8.1 and Windows 10.** A Windows 7 version is on the works.

gca-node final code will be able to be compiled on Mac OS Sierra and Linux with kernel version from 3.16.
### Prerequisites
  * **Visual Studio 2015** or Visual Studio 2017 RC, although the former is strongly recommended.
  * A built DLL of **[libusb 1.0.21][1]** or higher. It can be built from source on Visual Studio 2015 by opening the solution `libusb_2015.sln` within the `msvc` folder and building the project `libusb1.0 (dll)`. 
    * You can also download a prebuilt DLL from the [libusb repository][2].
  * **ElMassivo's Wii U GCN USB Driver**, either [downloding the latest release][3] or [building it from source][4]. Carefully follow all the instructions to install a new driver for the Wii U GameCube&trade; Adapter so that Windows may correctly configure it.
### Build procedure
 1. Download or clone the gca-node repository.
 2. Create two directories within the `initial_tests\LibUSBTest<X>` folder called `lib`, where `<X>` is the number of test you want to try.
 3. Copy both `libusb-1.0.dll` and `libusb-1.0.lib` you've just built inside the new `lib` folder.
 4. Open `LibUSBTest<X>.sln` and compile it.
 5. The program should automatically copy the `libusb-1.0.dll` file within the executable program's folder. If it doesn't, Windows will report that the DLL is missing. You'll have to copy it manually to the folder where the executable is located.

### Pre-execution setup
1. Make sure you've installed **ElMassivo's Wii U GCN USB Driver** and changed the driver of the Wii U GameCube&trade; Adapter to WinUSB.
2. However, **do *not* execute the built test program** while the driver is running, since the driver will have claimed the interface to connect with the adapter. If you have start it for testing purposes, please turn it off.

[1]: https://sourceforge.net/projects/libusb/files/libusb-1.0/libusb-1.0.21/libusb-1.0.21.tar.bz2/download
[2]: https://github.com/libusb/libusb/releases/download/v1.0.21/libusb-1.0.21.tar.bz2
[3]: https://bitbucket.org/elmassivo/gcn-usb-adapter/downloads/WiiU-UsbSetup%202015-09-02.exe
[4]: https://bitbucket.org/elmassivo/gcn-usb-adapter/src