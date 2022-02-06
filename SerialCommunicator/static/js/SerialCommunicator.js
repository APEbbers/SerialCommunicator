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

        self.EnableSwitch = function() {
            if (self.settings.settings.plugins.SerialCommunicator.Switch.EnableOnOffBtn() == false) {
                document.getElementById("swicth_container").remove();
            }
        };

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

            return true; //Thus must be true. Otherwise you need to click extra, and the function will get out of sink with the toggle button.
        };

        // self.getExamplesJS = function () {
        //     // const url = `https://api.github.com/repos/${user}/${repo}/git/trees/master`;
        //     // const list = await fetch(url).then(res => res.json());
        //     // const dir = list.tree.find(node => node.path === directory);
        //     // if (dir) {
        //     //     const list = await fetch(dir.url).then(res => res.json());
        //     //     return list.tree.map(node => node.path);
        //     // }
        //     // const list = await octokit.request('GET /repos/{owner}/{repo}/contents/{path}', {
        //     //     owner: 'APEbbers',
        //     //     repo: 'SerialCommunicator',
        //     //     path: 'Examples'
        //     // });
        //     // const dir = list.tree.find(node => node.path === directory);
        //     OctoPrint.simpleApiCommand("SerialCommunicator", "ExamplesJS", { "ip": "test"})
        //     // if (dir) {
        //     //     const list = await fetch(dir.url).then(res => res.json());
                
        //     //     // OctoPrint.simpleApiCommand("SerialCommunicator", "ExamplesJS", { "ip": list.tree.map(node => node.path) });
        //     //     OctoPrint.simpleApiCommand("SerialCommunicator", "ExamplesJS", { "ip": "test"})
        //     //     //self.settings.settings.plugins.SerialCommunicator.Examples(list.tree.map(node => node.path))
        //     //     // return list.tree.map(node => node.path);
        //     // };
        // };

        self.onBeforeBinding = function() {     
            // Run these functions before binding to set the correct settings at startup.
            self.switchLED();
            self.EnableSwitch();
            // self.ExampleSettings(self.settings.settings.plugins.SerialCommunicator.Examples());
        }
    };
    ADDITIONAL_VIEWMODELS.push({
        construct: SerialCommunicatorViewModel,
        dependencies: ["settingsViewModel", "navigationViewModel"],
        elements: ["#navbar_plugin_SerialCommunicator", "#settings_plugin_SerialCommunicator"],
    });
});

