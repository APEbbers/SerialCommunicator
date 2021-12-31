# coding=utf-8
from __future__ import absolute_import, unicode_literals

import octoprint.plugin
import serial
import usb.core
import usb.backend.libusb1

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
            # selectedPort2={"USB1", "USB2", "USB3", "USB4"},
            selectedPort2="",
            )

    def get_template_configs(self):
        return [
                dict(type="settings", custom_bindings=False)
                ]

    # def get_assets(self):
    #     self._logger.info("Get javascript knockout")
    #     return dict(
    #         js=["js/SerialCommunicator.js"]
    #     )

    # def get_template_vars(self):
    #     return dict(selectedPort2=self._settings.get(["selectedPort2"]))

    def get_template_vars(self):
        import re
        import subprocess
        device_re = re.compile(b"Bus\s+(?P<bus>\d+)\s+Device\s+(?P<device>\d+).+ID\s(?P<id>\w+:\w+)\s(?P<tag>.+)$", re.I)
        df = subprocess.check_output("lsusb")
        devices = []
        for i in df.split(b'\n'):
            if i:
                info = device_re.match(i)
                if info:
                    dinfo = info.groupdict()
                    dinfo['device'] = '/dev/bus/usb/%s/%s' % (dinfo.pop('bus'), dinfo.pop('device'))
                    devices.append(dinfo)                    
        print(devices)           
        objItems=devices
        self._logger.debug("In CreateList")
        # objItems = {
        #             'Blue',
        #             'Red',
        #             'White',
        #             'Green',
        #             'Black',
        #             'Orange',
        #     }
        return dict(selectedPort2=objItems)

    def on_settings_save(self, data):
        octoprint.plugin.SettingsPlugin.on_settings_save(self, data)

    def HandleM150(
                    self, comm_instance, phase, cmd, cmd_type, gcode,
                    *args, **kwargs):
        c = self._settings.get(["Command1"])

        if cmd == c:
            self._logger.debug(f"{c} Detected!")
            ser = serial.Serial(self._settings.get(["selectedPort"]))
            ser.baudrate = int(self._settings.get(["selectedBaudrate"]))
            ser.timeout = 10
            ser.write(bytes(self._settings.get(["Command1"]), 'utf-8'))
            # ser.close()

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
# __plugin_pythoncompat__ = ">=3,<4"  # only python 3
__plugin_pythoncompat__ = ">=2.7,<4"  # python 2 and 3


def __plugin_load__():
    global __plugin_implementation__
    __plugin_implementation__ = SerialcommunicatorPlugin()

    global __plugin_hooks__
    __plugin_hooks__ = {
        "octoprint.plugin.softwareupdate.check_config":
        __plugin_implementation__.get_update_information,
        "octoprint.comm.protocol.gcode.queuing":
        __plugin_implementation__.HandleM150,
    }
