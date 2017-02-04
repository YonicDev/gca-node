![gca-node logo][logo]

gca-node is a NodeJS addon that adds Nintendo&reg; Wii U GameCube&trade; Adapter native support for NodeJS applications.

## Usage

**gca-node currently only builds on Windows 7+**, although there is planned support for Linux and Mac OS.
As of now, it can only be used on NW.js.

### Prerequisites
  * **Visual Studio 2015** with standard C++ tools.
  * **Node 7.5.0** or higher, with npm installed.
  * **NW.js SDK version.**
  * **Zadig**: You must use it to replace the HID driver installed on Windows for the adapter to a generic WinUSB one.
  
### Building

 1. Download or clone the gca-node repository.
 2. Install `node-gyp` globally (`npm install node-gyp -g`)
 3. Open the CLI inside the repository and execute `node-gyp rebuild`.

### NW.js
1. Copy the included `configure.bat` to your NW.js project.
2. Open the CLI inside your NW.js project, and run the `configure.bat` file to install the addon.
3. Whenever you need to use gca-node, use an alias for require so that other NodeJS tools like webpack do not mistake it as a NodeJS module.
   ```
      var native_require = eval('require');
      gca_node = native_require('gca-node.node')
   ```

## gca-node API

### Setup()
Detects the first Nintendo&reg; Wii U GameCube&trade; Adapter connected in your computer.
Returns 0 if the setup has detected one.

### Load()
Claims the interface to be used on the NodeJS application.
Returns 0 if an interface has been succesfully claimed.

### Read()
TODO. Returns an object with the current status of the GameCube controller.

### Request()
Obtains debug information about the current status of the GameCube controllers.


### Unload()
Releases the interface so that it can be used by other applications.
Returns 0 if succesful.

### Shutdown()
Closes the Nintendo&reg; Wii U GameCube&trade; Adapter so that it can be safely unplugged.
Returns 0 if succesful.

[logo]: http://i.imgur.com/quWt3jK.png