# coding=utf-8
from __future__ import absolute_import

import calendar
import time
from logging import exception

import octoprint.plugin
import serial
import serial.tools.list_ports
from github import Github, RateLimitExceededException
from octoprint.util import comm as comm

try:
    import termios
except ImportError:
    print("import failed")
import platform


class SerialCommunicatorPlugin(
        octoprint.plugin.StartupPlugin,
        octoprint.plugin.TemplatePlugin,
        octoprint.plugin.AssetPlugin,
        octoprint.plugin.EventHandlerPlugin,
        octoprint.plugin.SimpleApiPlugin,
        octoprint.plugin.SettingsPlugin):

    # Simple API plugin
    def on_after_startup(self):
        self._logger.info("SerialCommunicator")

    def get_api_commands(self):
        self._logger.debug(f'{"Manually triggered get_api."}')
        return dict(Switched=["ip"])

    def on_api_command(self, command, data):
        if command == 'Switched':
            self._logger.debug(f"{data}")
            if 'True' in str(data):
                self.SendSerialMessage("SwitchOn")
                self._logger.debug(f'{"Switched on!"}')
                # self.getPorts()
            if 'False' in str(data):
                self.SendSerialMessage("SwitchOff")
                self._logger.debug(f'{"Switched off!"}')
                # self.getPorts()
    # ----------------------------------------------------------

    def get_settings_defaults(self):
        return {
            "connection": {
                "selectedPort": "",
                "selectedBaudrate": "9600",
                "DeviceID": "Arduino #1",
                "timeout": "15"
            },
            "gcode": {
                "gcode_commands": "",
            },
            "action": {
                "action_any": "",
                "action_start": "",
                "action_cancel": "",
                "action_pause": "",
                "action_paused": "",
                "action_resume": "",
                "action_resumed": "",
                "action_disconnect": "",
                "action_notification": "",
                "action_sd_inserted": "",
                "action_sd_ejected": "",
                "action_sd_updated": "",
            },
            "returned_lines": {
                "string": "",
            },
            "events": {
                "Startup": False,
                "Shutdown": False,
                "ClientOpened": False,
                "ClientAuthed": False,
                "ClientClosed": False,
                "UserLoggedIn": False,
                "UserLoggedOut": False,
                "ConnectivityChanged": False,
                "Connecting": False,
                "Connected": False,
                "Disconnecting": False,
                "Disconnected": False,
                "Error": False,
                "PrinterStateChanged": False,
                "Upload": False,
                "FileAdded": False,
                "FileRemoved": False,
                "FolderAdded": False,
                "FolderRemoved": False,
                "UpdatedFiles": False,
                "MetadataAnalysisStarted": False,
                "MetadataAnalysisFinished": False,
                "FileSelected": False,
                "FileDeselected": False,
                "TransferStarted": False,
                "TransferDone": False,
                "PrintStarted": False,
                "PrintFailed": False,
                "PrintDone": False,
                "PrintCancelling": False,
                "PrintCancelled": False,
                "PrintPaused": False,
                "PrintResumed": False,
                "PowerOn": False,
                "PowerOff": False,
                "Home": False,
                "ZChange": False,
                "Dwell": False,
                "Waiting": False,
                "Cooling": False,
                "Alert": False,
                "Conveyor": False,
                "Eject": False,
                "EStop": False,
                "FilamentChange": False,
                "ToolChange": False,
                "CommandSuppressed": False,
                "InvalidToolReported": False,
                "CaptureStart": False,
                "CaptureDone": False,
                "CaptureFailed": False,
                "MovieRendering": False,
                "MovieDone": False,
                "MovieFailed": False,
                "SlicingStarted": False,
                "SlicingDone": False,
                "SlicingCancelled": False,
                "SlicingFailed": False,
                "SlicingProfileAdded": False,
                "SlicingProfileModified": False,
                "SlicingProfileDeleted": False,
                "SettingsUpdated": False,
                "PrinterProfileModified": False,
            },
            "Examples": list(),
            "Switch": {
                "EnableOnOffBtn": True,
                "Color": "",
                "State": False,
                "IconOn": "",
                "IconOff": "",
                "CurrentIcon": "",
                "ColorOn": "",
                "ColorOff": "",
                "CurrentColor": "",
                "SliderOff": "",
                "SliderOn": "",
                "CurrentSilder": "",
            }
        }

    def get_template_configs(self):
        return [
            dict(type="navbar", custom_bindings=True),
            dict(type="settings", custom_bindings=False)
        ]

    def get_assets(self):
        return dict(
            css=["css/SerialCommunicator.css"],
            js=["js/SerialCommunicator.js"],
        )

    def get_template_vars(self):
        return dict(selectedPort=self.getPorts(), Examples=self.getExamples())

    # Get the ports for the jinja2 template
    def getPorts(self):
        ports = serial.tools.list_ports.comports()  # get all the active ports
        result = []
        # Get the connection between printer and octoprint
        PrinterConnection = self._printer.get_current_connection()

        for port, desc, hwid, in sorted(ports):
            try:
                StringA = str(hwid)
                hwID = StringA[:StringA.index("LOCATION") - 1]
            except Exception:
                hwID = hwid
            # If the port is not equal to the connection between octoprint and the printer, add it to the list.
            if port != PrinterConnection[1]:
                self._logger.debug(f"port adress sent to 'readID' is: {port}")

                # add strip to remove hidden surroundong char and/or whitespace
                IDstring = str(self.ReadID(port, "GetID")).strip()
                IDSetting = str(self._settings.get(
                    ["connection", "DeviceID"])).strip()  # add strip to remove hidden surroundong char and/or whitespace

                self._logger.debug(
                    f"The ID string is: {IDstring}, in settings: {IDSetting}, comparison result: {IDstring in IDSetting}")

                # If the ID of the device is equal to the ID in settings, add the device to the list.
                if IDstring in IDSetting:
                    self._logger.debug(f"{IDstring} is added to the port list")
                    result.append("{}#{}#[{}]".format(port, desc, hwID))
                else:
                    self._logger.debug(
                        f"{IDstring} is not added to the port list")
        result.append("VIRTUAL")
        ports.clear()

        objItems = result
        return objItems

    # get example sketches for a arduino board from a github repository.
    def getExamples(self):
        LinkArray = []
        try:
            g = Github()
            repo = g.get_repo("APEbbers/SerialCommunicator")
            contents = repo.get_contents("Examples")
            repo_url = str(repo.html_url).replace('.git', '') + '/tree/master/'

            while len(contents) > 0:
                file_content = contents.pop(0)
                if file_content.type == 'dir':
                    contents.extend(repo.get_contents(file_content.path))
                else:
                    path = str(file_content.path).replace(' ', '%20')
                    self._logger.debug(repo_url + path)
                    LinkArray.append(f"{repo_url + path}#{file_content.name}")
        except RateLimitExceededException:
            search_rate_limit = g.get_rate_limit().search
            reset_timestamp = calendar.timegm(
                search_rate_limit.reset.timetuple())
            # add 10 seconds to be sure the rate limit has been reset
            sleep_time = reset_timestamp - calendar.timegm(time.gmtime()) + 10

            self._logger.info(
                f"Too many requests to the GitHub repository! Please retry over {sleep_time}")
            LinkArray.append(
                f'https://github.com/#Too many requests to the GitHub repository! Please refresh over {sleep_time} seconds')
        return LinkArray

    def on_settings_save(self, data):
        octoprint.plugin.SettingsPlugin.on_settings_save(self, data)

    def ReadID(self, objPort, message):
        if platform.system() == 'Linux':
            try:
                # Solution to open the port properly (bug in PySerial):
                # https://stackoverflow.com/questions/15460865/disable-dtr-in-pyserial-from-code/15479088#15479088
                # https://raspberrypi.stackexchange.com/questions/9695/disable-dtr-on-ttyusb0/31298#31298
                f = open(objPort)
                attrs = termios.tcgetattr(f)
                attrs[2] = attrs[2] & ~termios.HUPCL
                termios.tcsetattr(f, termios.TCSAFLUSH, attrs)
                f.close()
                # --------------------------------------------------------------------------------------------------
            except exception:
                self._logger.debug(f"{exception} resieved.")

        try:
            ser = serial.Serial(objPort, int(self._settings.get(
                ["connection", "selectedBaudrate"])), timeout=1)
            ser.setDTR(False)
            time.sleep(0.5)

            self._logger.debug(
                f"port is: {ser.port}, baudrate is: {ser.baudrate}")

            ser.reset_input_buffer()

            # this will store the line
            seq = []
            count = 0

            while ser.is_open:
                ser.write(str(message).encode('ascii'))
                time.sleep(0.5)
                for c in ser.read():
                    seq.append(chr(c))  # convert from ANSII
                    # Make a string from array
                    joined_seq = ''.join(str(v) for v in seq)

                    # set an tineout
                    tic = time.time()
                    tout = int(self._settings.get(["connection", "timeout"]))
                    while ((time.time() - tic) < tout):
                        if chr(c) == '\n':

                            returnMessage = joined_seq  # create the message
                            seq = []

                            count += 1
                            if (count == 2):
                                ser.close()

                                self._logger.debug(
                                    f"{returnMessage} read. port={ser.port}, baudrate={ser.baudrate}, timer stopped at: {(time.time() - tic)} seconds")

                                return returnMessage  # return the message
                    else:
                        self._logger.debug(f"timeout after {tout} seconds")
                        count += 1
                        if (count == 2):  # iafter 2 times, the code stops
                            break
                        continue
            ser.close()
        except serial.SerialTimeoutException as e:
            self._logger.debug("TimeOut exception!!")
            self._logger.debug(e)
            ser.close()
        except serial.SerialException as e:
            self._logger.debug("Serial exception!!")
            self._logger.debug(e)
            ser.close()

    # send the message to the arduino over the selected (usb) serial port.
    def SendSerialMessage(self, message):
        # Get the connection between printer and octoprint
        PrinterConnection = self._printer.get_current_connection()
        self._logger.debug(f"Printer port is: {PrinterConnection[1]}")
        result = []
        self._logger.debug(f"{message} recieved in handler.")

        ports = serial.tools.list_ports.comports()  # get al the active ports
        for port, desc, hwid, in sorted(ports):
            try:
                StringA = str(hwid)
                hwID = StringA[:StringA.index("LOCATION") - 1]
            except Exception:
                hwID = hwid
            # If the port is not equal to the connection between octoprint and the printer, add it to the list.
            if port != PrinterConnection[1]:
                result.append("{}#{}#[{}]".format(port, desc, hwID))
        ports.clear()
        for item in result:
            objHWid = str(item).split("#")[2]
            objPort = str(item).split("#")[0]
            # if the hardware id is equal to the one saved under config.yaml (settings), send the message over serial.
            if objHWid == self._settings.get(["connection", "selectedPort"]):
                objPort = str(item).split("#")[0]

            if platform.system() == 'Linux':
                try:
                    # Solution to open the port properly (bug in PySerial):
                    # https://stackoverflow.com/questions/15460865/disable-dtr-in-pyserial-from-code/15479088#15479088
                    # https://raspberrypi.stackexchange.com/questions/9695/disable-dtr-on-ttyusb0/31298#31298
                    f = open(objPort)
                    attrs = termios.tcgetattr(f)
                    attrs[2] = attrs[2] & ~termios.HUPCL
                    termios.tcsetattr(f, termios.TCSAFLUSH, attrs)
                    f.close()
                    # --------------------------------------------------------------------------------------------------
                except exception:
                    self._logger.debug(f"{exception} resieved.")

                ser = serial.Serial(objPort, int(self._settings.get(
                    ["connection", "selectedBaudrate"])), timeout=1)
                ser.setDTR(False)
                time.sleep(0.5)

                self._logger.debug(f"hwID: {objHWid}, port: {objPort}")
                try:
                    ser.open()
                    ser.reset_input_buffer()
                    ser.write(str(message).encode('ascii'))
                    if ser.isOpen() is True:
                        self._logger.debug(
                            f"{message} sent. port={ser.port}, baudrate={ser.baudrate}")
                    if ser.isOpen() is False:
                        self._logger.debug(
                            f"Serial is closed! port={ser.port}, baudrate={ser.baudrate}")
                    ser.close()
                except serial.SerialException:
                    ser.close()

    def HandleGCODE(self, comm_instance, phase, cmd, cmd_type, gcode, *args, **kwargs):
        c = self._settings.get(['gcode', 'gcode_commands'])
        gcodeArray = str(c).split(";")

        if gcode in gcodeArray:
            if "VIRTUAL" in self._settings.get(["connection", "selectedPort"]):
                self._logger.debug("No serial connection")
            else:
                self.SendSerialMessage(gcode)
                self._logger.debug(f"gcode: {gcode} sent.")

    def HandleResponse(self, comm_instance, line, *args, **kwargs):
        c = self._settings.get(['returned_lines', 'string'])
        gcodeArray = str(c).split(";")
        for objString in gcodeArray:
            if objString != "" and objString in line:
                if "VIRTUAL" in self._settings.get(["connection", "selectedPort"]):
                    self._logger.debug(
                        "No serial connection -> {objString} Detected in {line} ")
                else:
                    self.SendSerialMessage(objString)
                    self._logger.debug(f"word(s): {objString} sent.")
        return line

    def HandleActionMessage(self, comm, line, action, name, params, *args, **kwargs):
        # self._logger.debug(f"action name: {name}, action message: {line} recieved bij OctoPrint")

        action_any = "any;" + self._settings.get(['action', 'action_any'])
        action_start = "start;" + \
            self._settings.get(['action', 'action_start'])
        action_cancel = "cancel;" + \
            self._settings.get(['action', 'action_cancel'])
        action_pause = "pause;" + \
            self._settings.get(['action', 'action_pause'])
        action_paused = "paused;" + \
            self._settings.get(['action', 'action_paused'])
        action_resume = "resume;" + \
            self._settings.get(['action', 'action_resume'])
        action_resumed = "resumed;" + \
            self._settings.get(['action', 'action_resumed'])
        action_disconnect = "disconnect;" + \
            self._settings.get(['action', 'action_disconnect'])
        action_notification = "notification;" + \
            self._settings.get(['action', 'action_notification'])
        action_sd_inserted = "sd_inserted;" + \
            self._settings.get(['action', 'action_sd_inserted'])
        action_sd_ejected = "sd_ejected;" + \
            self._settings.get(['action', 'action_sd_ejected'])
        action_sd_updated = "sd_updated;" + \
            self._settings.get(['action', 'action_sd_updated'])

        LinesArray = [action_any, action_start, action_cancel, action_pause, action_paused, action_resume,
                      action_resumed, action_disconnect, action_notification, action_sd_inserted, action_sd_ejected, action_sd_updated]
        for action_line in LinesArray:
            action_name = str(action_line).split(";")[0]
            action_messages = str(action_line).split(";")[1].split(";")
            # self._logger.debug(f"action name: {action_name}, action message: {action_message}")
            if action_messages != "":
                for action_message in action_messages:
                    if action_name == name and action_name != "any" and action_message in action:
                        if self._settings.get(["connection", "selectedPort"]) == "VIRTUAL":
                            self._logger.debug("No serial connection")
                        else:
                            self.SendSerialMessage(action_name)
                        self._logger.info(f"action name: {action_name} sent.")
                    if action_name == "any" and action_message in action:
                        if self._settings.get(["connection", "selectedPort"]) == "VIRTUAL":
                            self._logger.debug("No serial connection")
                        else:
                            self.SendSerialMessage(action_name)
                        self._logger.info(f"action line: {action_name} sent.")

    # Catch octoprint events and send them over the serial connection.
    def on_event(self, event, payload):
        eventlist = {"Startup", "Shutdown", "ClientOpened", "ClientAuthed", "ClientClosed", "UserLoggedIn", "UserLoggedOut", "ConnectivityChanged", "Connecting", "Connected",
                     "Disconnecting", "Disconnected", "Error", "PrinterStateChanged", "Upload", "FileAdded", "FileRemoved", "FolderAdded", "FolderRemoved", "UpdatedFiles",
                     "MetadataAnalysisStarted", "MetadataAnalysisFinished", "FileSelected", "FileDeselected", "TransferStarted", "TransferDone", "PrintStarted", "PrintFailed",
                     "PrintDone", "PrintCancelling", "PrintCancelled", "PrintPaused", "PrintResumed", "PowerOn", "PowerOff", "Home", "ZChange", "Dwell", "Waiting", "Cooling",
                     "Alert", "Conveyor", "Eject", "EStop", "FilamentChange", "ToolChange", "CommandSuppressed", "InvalidToolReported", "CaptureStart", "CaptureDone",
                     "CaptureFailed", "MovieRendering", "MovieDone", "MovieFailed", "SlicingStarted", "SlicingDone", "SlicingCancelled", "SlicingFailed", "SlicingProfileAdded",
                     "SlicingProfileModified", "SlicingProfileDeleted", "SettingsUpdated", "PrinterProfileModified"}

        for eventname in eventlist:
            if self._settings.get(["events", eventname]):
                eventParameter = self._settings.get(['events', eventname])
                self._logger.debug(
                    f"eventname: {eventname}, event: {event}, eventParameter: {eventParameter}")
                if eventname == event and eventParameter is not False:
                    self._logger.debug(f"event: {event}detected!")

                    if self._settings.get(["connection", "selectedPort"]) == "VIRTUAL":
                        self._logger.debug("No serial connection")
                    else:
                        self.SendSerialMessage(str(event))
                        self._logger.info(f"event: {event} sent.")

    def LightSwitch(self):
        c = self._settings.get(['Switch', 'State'])
        objresult = None
        if c is False:
            self.SendSerialMessage("SwitchOff")
            objresult = True
            self._logger.debug("SwitchOff")
        if c is True:
            self.SendSerialMessage("SwitchOn")
            objresult = False
            self._logger.debug("SwitchOn")

    def on_settings_load(self):
        data = octoprint.plugin.SettingsPlugin.on_settings_load(self)
        return data

    def get_update_information(self):
        # Define the configuration for your plugin to use with the Software Update
        # Plugin here. See:
        #  https://docs.octoprint.org/en/master/bundledplugins/softwareupdate.html
        # for details.
        return dict(
            SerialCommunicator=dict(
                displayName="Serialcommunicator Plugin",
                displayVersion=self._plugin_version,

                # version check: github repository
                type="github_release",
                user="APEbbers",
                repo="SerialCommunicator",
                current=self._plugin_version,

                # update method: pip
                pip="https://github.com/APEbbers/SerialCommunicator/archive/, \
                {target_version}.zip"
            )
        )


__plugin_name__ = "SerialCommunicator Plugin"


# __plugin_pythoncompat__ = ">=2.7,<3"  # only python 2
__plugin_pythoncompat__ = ">=3,<4"  # only python 3
# __plugin_pythoncompat__ = ">=2.7,<4"  # python 2 and 3


def __plugin_load__():
    global __plugin_implementation__
    __plugin_implementation__ = SerialCommunicatorPlugin()

    global __plugin_hooks__
    __plugin_hooks__ = {
        "octoprint.plugin.softwareupdate.check_config":
        __plugin_implementation__.get_update_information,
        "octoprint.comm.protocol.gcode.received":
        __plugin_implementation__.HandleResponse,
        "octoprint.comm.protocol.gcode.sent":
        __plugin_implementation__.HandleGCODE,
        "octoprint.comm.protocol.action":
        __plugin_implementation__.HandleActionMessage,
    }
