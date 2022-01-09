# coding=utf-8
from __future__ import absolute_import
from octoprint.events import eventManager, Events

import octoprint.plugin
import serial.tools.list_ports
from octoprint.util import comm as comm


class SerialCommunicatorPlugin(
        octoprint.plugin.StartupPlugin,
        octoprint.plugin.TemplatePlugin,
        octoprint.plugin.AssetPlugin,
        octoprint.plugin.EventHandlerPlugin,
        octoprint.plugin.SettingsPlugin):

    def on_after_startup(self):
        self._logger.info("SerialCommunicator")

    def get_settings_defaults(self):
        return {
            "connection": {
                "selectedPort": "",
                "selectedBaudrate": "",
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
            }
        }

    def get_template_configs(self):
        return [
            dict(type="settings", custom_bindings=False)
        ]

    def get_assets(self):
        return dict(
            css=["css/SerialCommunicator.css"]
        )

    def get_template_vars(self):
        ports = serial.tools.list_ports.comports()
        result = []

        for port, desc, hwid, in sorted(ports):
            try:
                StringA = str(hwid)
                hwID = StringA[:StringA.index("LOCATION")-1]
            except Exception:
                hwID = hwid
            result.append("{}#{}#[{}]".format(port, desc, hwID))
        result.append("VIRTUAL_For testing")

        objItems = result
        return dict(selectedPort=objItems)

    def on_settings_save(self, data):
        octoprint.plugin.SettingsPlugin.on_settings_save(self, data)

    def SendSerialMessage(self, message):
        ports = serial.tools.list_ports.comports()
        result = []
        self._logger.debug(f"{message} recieved in handler.")

        for port, desc, hwid, in sorted(ports):
            try:
                StringA = str(hwid)
                hwID = StringA[:StringA.index("LOCATION")-1]
            except Exception:
                hwID = hwid
            result.append("{}#{}#[{}]".format(port, desc, hwID))
        for item in result:
            hwID = str(item).split("#")[2]
            port = str(item).split("#")[0]
            if hwID == self._settings.get(["connection", "selectedPort"]):
                self._logger.debug(f"hwID: {hwID}, port: {port}")
                ser = serial.Serial(port)
                ser.baudrate = int(self._settings.get(
                    ["connection", "selectedBaudrate"]))
                ser.timeout = 10
                ser.write(bytes(message, 'utf-8'))
                ser.close()
                self._logger.info(f"{message} sent.")

    def HandleGCODE(self, comm_instance, phase, cmd, cmd_type, gcode, *args, **kwargs):
        c = self._settings.get(['gcode', 'gcode_commands'])
        gcodeArray = str(c).split(";")
        # self._logger.info(f"{gcode} Detected in {c}. {gcode} will be passed!.")
        if gcode in gcodeArray:
            if self._settings.get(["connection", "selectedPort"]) == "VIRTUAL":
                self._logger.debug("No serial connection")
            else:
                self.SendSerialMessage(gcode)
                self._logger.info(f"gcode: {gcode} sent.")

    def HandleActionMessage(self, comm, line, action, name, params, *args, **kwargs):
        action_any = "any;"+str(self._settings.get(['action', 'action_any']))
        action_start = "start;" + \
            str(self._settings.get(['action', 'action_start']))
        action_cancel = "cancel;" + \
            str(self._settings.get(['action', 'action_cancel']))
        action_pause = "pause;" + \
            str(self._settings.get(['action', 'action_pause']))
        action_paused = "paused;" + \
            str(self._settings.get(['action', 'action_paused']))
        action_resume = "resume;" + \
            str(self._settings.get(['action', 'action_resume']))
        action_resumed = "resumed;" + \
            str(self._settings.get(['action', 'action_resumed']))
        action_disconnect = "disconnect;" + \
            str(self._settings.get(['action', 'action_disconnect']))
        action_notification = "notification;" + \
            str(self._settings.get(['action', 'action_notification']))
        action_sd_inserted = "sd_inserted;" + \
            str(self._settings.get(['action', 'action_sd_inserted']))
        action_sd_ejected = "sd_ejected;" + \
            str(self._settings.get(['action', 'action_sd_ejected']))
        action_sd_updated = "sd_updated;" + \
            str(self._settings.get(['action', 'action_sd_updated']))

        MessageArray = [action_any, action_start, action_cancel, action_pause, action_paused, action_resume,
                        action_resumed, action_disconnect, action_notification, action_sd_inserted, action_sd_ejected, action_sd_updated]
        for Message in MessageArray:
            action_name = str(Message).split(";")[0]
            action_message = str(Message).split(";")[1]
            action_line = Message
            self._logger.debug(f"action name: {action_name}, action message: {action_message}")
            if action_message != "":
                if action_name == name and action_name != "any" and action_message in line:
                    if self._settings.get(["connection", "selectedPort"]) == "VIRTUAL":
                        self._logger.debug("No serial connection")
                    else:
                        self.SendSerialMessage(action_name)
                    self._logger.info(f"action name: {action_name} sent.")
                if action_name == "any" and action_message in action:
                    if self._settings.get(["connection", "selectedPort"]) == "VIRTUAL":
                        self._logger.debug("No serial connection")
                    else:
                        self.SendSerialMessage(action_line)
                    self._logger.info(f"action line: {action_line} sent.")

    def on_event(self, event, payload):
        eventlist = {"Startup", "Shutdown", "ClientOpened", "ClientAuthed", "ClientClosed", "UserLoggedIn", "UserLoggedOut", "ConnectivityChanged", "Connecting", "Connected", "Disconnecting", "Disconnected", "Error", "PrinterStateChanged", "Upload", "FileAdded", "FileRemoved", "FolderAdded", "FolderRemoved", "UpdatedFiles", "MetadataAnalysisStarted", "MetadataAnalysisFinished", "FileSelected", "FileDeselected", "TransferStarted", "TransferDone", "PrintStarted", "PrintFailed", "PrintDone", "PrintCancelling",
                     "PrintCancelled", "PrintPaused", "PrintResumed", "PowerOn", "PowerOff", "Home", "ZChange", "Dwell", "Waiting", "Cooling", "Alert", "Conveyor", "Eject", "EStop", "FilamentChange", "ToolChange", "CommandSuppressed", "InvalidToolReported", "CaptureStart", "CaptureDone", "CaptureFailed", "MovieRendering", "MovieDone", "MovieFailed", "SlicingStarted", "SlicingDone", "SlicingCancelled", "SlicingFailed", "SlicingProfileAdded", "SlicingProfileModified", "SlicingProfileDeleted", "SettingsUpdated", "PrinterProfileModified"}
        
        for eventname in eventlist:
            if self._settings.get(["events", eventname]):
                eventParameter = self._settings.get(['events', eventname])
                self._logger.debug(f"eventname: {eventname}, event: {event}, eventParameter: {eventParameter}")
                if eventname == event and eventParameter != False:
                    self._logger.debug(f"event: {event}detected!")

                    if self._settings.get(["connection", "selectedPort"]) == "VIRTUAL":
                        self._logger.debug("No serial connection")
                    else:
                        self.SendSerialMessage(str(event))
                        self._logger.info(f"event: {event} sent.")

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
# __plugin_pythoncompat__ = ">=3,<4"  # only python 3
__plugin_pythoncompat__ = ">=2.7,<4"  # python 2 and 3


def __plugin_load__():
    global __plugin_implementation__
    __plugin_implementation__ = SerialCommunicatorPlugin()

    global __plugin_hooks__
    __plugin_hooks__ = {
        "octoprint.plugin.softwareupdate.check_config":
        __plugin_implementation__.get_update_information,
        "octoprint.comm.protocol.gcode.sent":
        __plugin_implementation__.HandleGCODE,
        "octoprint.comm.protocol.action":
        __plugin_implementation__.HandleActionMessage,
    }
