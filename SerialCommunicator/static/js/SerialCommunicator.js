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
                
        self.switchLED = function () {
            document = self; 
            checkbox = document.getElementById("light_switch");
            if (checkbox.checked == true) document.getElementById("SwitchIcon").classList.replace('far', 'fas');
            if (checkbox.checked != true) document.getElementById("SwitchIcon").classList.replace('fas', 'far');

            OctoPrint.simpleApiCommand("SerialCommunicator", "Switched", { "ip": checkbox.checked });
            self.settings.settings.plugins.SerialCommunicator.Switch.State(checkbox.checked);
            return true; //Thus must be true. Otherwise you need to click extra, and the function will get out of sink with the toggle button.
        };        
    };

    ADDITIONAL_VIEWMODELS.push([
        SerialCommunicatorViewModel,
        ["settingsViewModel"],
        ["#navbar_plugin_SerialCommunicator"],
    ]);
});

