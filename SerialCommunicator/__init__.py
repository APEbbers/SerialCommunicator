# coding=utf-8
from __future__ import absolute_import, unicode_literals

import octoprint
import octoprint.plugin
import serial
import serial.tools.list_ports
from octoprint.util import comm as comm


class SerialcommunicatorPlugin(
        octoprint.plugin.StartupPlugin,
        octoprint.plugin.TemplatePlugin,
        octoprint.plugin.SettingsPlugin):

    def on_after_startup(self):
        self._logger.info("SerialCommunicator")

    def get_settings_defaults(self):
        return dict(
            selectedPort="",
            selectedBaudrate="",
            Command1="",
        )

    def get_template_configs(self):
        return [
            dict(type="settings", custom_bindings=False)
        ]

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

    def HandleM150(self, comm_instance, phase, cmd, cmd_type, gcode, *args, **kwargs):
        c = self._settings.get(["Command1"])
        if gcode in c:
            if self._settings.get(["selectedPort"]) == "VIRTUAL":
                self._logger.info(f"{gcode} Detected in {c}. {gcode} will be passed!.")
            else:
                ports = serial.tools.list_ports.comports()
                result = []

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
                    if hwID == self._settings.get(["selectedPort"]):
                        self._logger.debug(f"hwID: {hwID}, port: {port}")
                        ser = serial.Serial(port)
                        ser.baudrate = int(self._settings.get(["selectedBaudrate"]))
                        ser.timeout = 10
                        ser.write(bytes(gcode, 'utf-8'))
                        ser.close()

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


__plugin_name__ = "Serialcommunicator Plugin"


# __plugin_pythoncompat__ = ">=2.7,<3"  # only python 2
__plugin_pythoncompat__ = ">=3,<4"  # only python 3
# __plugin_pythoncompat__ = ">=2.7,<4"  # python 2 and 3


def __plugin_load__():
    global __plugin_implementation__
    __plugin_implementation__ = SerialcommunicatorPlugin()

    global __plugin_hooks__
    __plugin_hooks__ = {
        "octoprint.plugin.softwareupdate.check_config":
        __plugin_implementation__.get_update_information,
        "octoprint.comm.protocol.gcode.sent":
        __plugin_implementation__.HandleM150,
    }
