/*
 * View model for SerialCommunicator
 *
 * Author: A.P. Ebbers
 * License: AGPLv3
 */
$(function () {
    function SerialCommunicatorViewModel(parameters) {
        var self = this;

        self.settings = parameters[0];
        
        self.onBeforeBinding = function() {
            // Run these functions before binding to set the correct settings at startup.
            self.switchLED();
            self.EnableSwitch();
        }

        self.EnableSwitch = function() {
            SwitchStatus = self.settings.settings.plugins.SerialCommunicator.Switch.EnableOnOffBtn();
            if (SwitchStatus == false) {
                document.getElementById("swicth_container").remove();
            }
        }

        self.switchLED = function () {
            document = self;
            checkbox = document.getElementById("light_switch");

            // Get the icons. If no Icons are defined in settings, set the default.
            IconOn = self.settings.settings.plugins.SerialCommunicator.Switch.IconOn();
            if (IconOn == "") IconOn = "fas fa-lightbulb";
            IconOff = self.settings.settings.plugins.SerialCommunicator.Switch.IconOff();
            if (IconOff == "") IconOff = "far fa-lightbulb";

            // Get the icon color. If no color is defined in settings, set the default.
            ColorOn = self.settings.settings.plugins.SerialCommunicator.Switch.ColorOn();
            if (ColorOn == "") ColorOn = "color:black";
            ColorOff = self.settings.settings.plugins.SerialCommunicator.Switch.ColorOff();
            if (ColorOff == "") ColorOff = "color:black";

            // Get the switch background color. If no color is defined in settings, set the default.
            SwitchColorOn = self.settings.settings.plugins.SerialCommunicator.Switch.SliderOn();
            if (SwitchColorOn == "") SwitchColorOn = "color:black";
            SwitchColorOff = self.settings.settings.plugins.SerialCommunicator.Switch.SliderOff();
            if (SwitchColorOff == "") SwitchColorOff = "color:black";

            // Change the icon based on checkbox.checked
            if (checkbox.checked == true) {
                document.getElementById("SwitchIcon").className = IconOn;
                document.getElementById("SwitchIcon").style["color"] = ColorOn;
                document.getElementById("light_slider").style["background-color"] = SwitchColorOn;
            }
            if (checkbox.checked != true) {
                document.getElementById("SwitchIcon").className = IconOff;
                document.getElementById("SwitchIcon").style["color"] = ColorOff;
                document.getElementById("light_slider").style["background-color"] = SwitchColorOff;
            }

            // Pass the state of the checkbox to _init_.py -> simpleApiCommand
            OctoPrint.simpleApiCommand("SerialCommunicator", "Switched", { "ip": checkbox.checked });
            // Save the state of the checkbox to settings.
            self.settings.settings.plugins.SerialCommunicator.Switch.State(checkbox.checked);

            return true; //Thus must be true. Otherwise you need to click extra, and the function will get out of sink with the toggle button.
        };
    };

    ADDITIONAL_VIEWMODELS.push({
        construct: SerialCommunicatorViewModel,
        dependencies: ["settingsViewModel", "navigationViewModel"],
        elements: ["#navbar_plugin_SerialCommunicator"],
    });
});

