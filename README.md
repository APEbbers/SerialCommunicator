# SerialCommunicator

This plugin let you send gcodes, action commands and events to an arduino. (or any other device). The arduino then can do something, for example change the color of a LED strip.
The plugin is developed with an usb serial connection in mind between a Raspberry PI and an arduino.


## Setup

Install via the bundled [Plugin Manager](https://docs.octoprint.org/en/master/bundledplugins/pluginmanager.html)
or manually using this URL:

    https://github.com/APEbbers/SerialCommunicator/archive/master.zip



## Configuration

After installing you will find the plugin under settings. 

![image](https://user-images.githubusercontent.com/10145631/149211063-4a215578-751f-4ce7-86d8-3f96cb8daa2b.png)


**Tab "Connection"**

![image](https://user-images.githubusercontent.com/10145631/149211146-05340218-3b55-437a-a883-88208128c27f.png)

Here can you:
- Select the USB port to which the arduino is connected. The USB port is identified with the VID and PID numbers of the connected device. 
  The plugin will uses these numbers to automaticly connect to the same device next time you start OctoPrint. Even when you plug your device to a different USB port.
- Select the baudrate. (The arduino must be programmed with the same baudrate!)


**Tab "GCODE commands"**

![image](https://user-images.githubusercontent.com/10145631/149211184-47827413-e2b9-450a-abeb-9bee31f2c993.png)

Enter an string of gcode commands, separated with ";" (For example: M500;M503;M81). When OctoPrint recieves one of these commands, the plugin will send these to the arduino.


**Tab "Action commands"**

![image](https://user-images.githubusercontent.com/10145631/149213259-ee1d68e7-61dd-4ded-b248-976cba8bfed4.png)

Here you can filter any of message belonging to one of the standard action commands. And action command looks like this: "// action:command some message"
For each command in the table you can enter an string of words separated with ";". (For example 0%;Finished;Error). When OctoPrint recieves a command and its message contains one of these words, the plugin sends the command to the arduino. (Only the command, without "// action:").
When you enter a string of words for the command "any", any action command with a message containing one of these words, triggers the plugin to send "any" to the arduino.


**Tab "Events"**

![image](https://user-images.githubusercontent.com/10145631/149213314-c6467c32-e6d5-452c-a7e5-555fbff1bb57.png)

Here you can check any of the standard events within OctoPrint. When checked and that events is triggered, the plugin will send the event name to Arduino. So when the event "connected" has occured, the plugin sends "connected" to arduino. Custom events are not implented (yet).


**Tab "Examples"**

![image](https://user-images.githubusercontent.com/10145631/149213882-e08d4a90-f8a0-4971-9ac5-232862cc482b.png)

Under the tab "Examples" you will find an table with links to program examples for arduino. 
It will show how to catch the messages send by this plugin and do something with it.

