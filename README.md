# SerialCommunicator

This plugin let you send gcodes, action commands and events to an arduino. (or any other device). The ardion then can do somethin. For example change the color of a LED strip.
The plugin is developed with an usb connection between a raspberry PI and an arduino in mind.

## Setup

Install via the bundled [Plugin Manager](https://docs.octoprint.org/en/master/bundledplugins/pluginmanager.html)
or manually using this URL:

    https://github.com/APEbbers/SerialCommunicator/archive/master.zip

**TODO:** Describe how to install your plugin, if more needs to be done than just installing it via pip or through
the plugin manager.

## Configuration

After installing you will find the plugin under settings. 
![image](https://user-images.githubusercontent.com/10145631/149208425-6e7094f9-438e-4253-b0da-7aafb2dcea9c.png)

tab "Connection":
![image](https://user-images.githubusercontent.com/10145631/149208805-e00c5211-9ac5-4df7-8f69-cc0943821694.png)
Here can you:
- Select the USB port to which the arduino is connected. The USB port is identified with VID and PID numbers. THe plugin will remember these numbers for next time and makes sure   
  that even when a different USB port is used, the correct USB port will be selected..
- Select the baudrate. (The arduino must be programmed with the same baudrate!)

tab "GCODE commands":
![image](https://user-images.githubusercontent.com/10145631/149210588-12df9e0b-8f40-464a-908e-31e2743130aa.png)
- Enter an string of gcode commands, separated with ";" (For example: M500;M503;M81). When OctoPrint recieves one of these commands, the plugin will send these to the arduino.

Under the tab "Examples" you will find an table with links to examples for programming arduino.

