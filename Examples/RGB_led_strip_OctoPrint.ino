/*
 Name:		RGB_led_strip_OctoPrint.ino
 Created:	1/14/2022 8:55:02 PM
 Author:	SKIKK
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN     13
#define NUM_LEDS    11
String message = ""; // Declare the message outside any void. Otherwise the message cannot be increased with lines 25.

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
#define DELAYVAL 0


// the setup function runs once when you press reset or power the board
void setup() { 
    delay(3000); // power-up safety delay

    strip.begin(); // Initialize NeoPixel strip object (REQUIRED)

    Serial.begin(9600); // Make the connection with bautrate 9600
}

// the loop function runs over and over again until power down or reset
void loop() {
    while (Serial.available() > 0)
    {
        strip.clear(); // Set all pixel colors to 'off'

        int inByte;
        inByte = Serial.read(); // Print the message for debugging
        //Serial.println(inByte);

        message = message + char(inByte); // Add the incoming byte to our message

        // Code for the On/Off switch on the navbar in octoprint-----------------
        if (message.endsWith("SwitchOn"))
        {
            EnableLEDStrip("Gray");
        }

        if (message.endsWith("SwitchOff"))
        {
            EnableLEDStrip("Black");
        }
        //-----------------------------------------------------------------------

        if (message.endsWith("M500"))
        {
            EnableLEDStrip("Purple");
        }

        if (message.endsWith("M503"))
        {
            EnableLEDStrip("Yellow");
        }  

        if (message.endsWith("uploaded"))
        {
            EnableLEDStrip("olive"); //non existing color
        }
            
        if (message.endsWith("SettingsUpdated") || message.endsWith("Finished"))
        {
            EnableLEDStrip("Green");
        }

        if (message.endsWith("Started") || message.endsWith("any"))
        {
            EnableLEDStrip("Gray");
        }

        if (message.endsWith("PrintCancelled") || message.endsWith("cancel"))
        {
            EnableLEDStrip("Red");
        }

        if (message.endsWith("PrintPaused") || message.endsWith("paused") || message.endsWith("disconnect"))
        {            
            EnableLEDStrip("orange");
        }
    }
}

void EnableLEDStrip(String objColor) {
    uint32_t objIndexColor = IndexColor(objColor);
    for (int i = 0; i < NUM_LEDS; i++)
    {        
        strip.setPixelColor(i, IndexColor(objColor)); //Set the color
        strip.show();   // Send the updated pixel colors to the hardware.
        delay(DELAYVAL); // Pause before next pass through loop
    }        
}
// Disable the colors you don't need to save memory.
uint32_t IndexColor(String objColor) {
    uint32_t RGBColor = strip.Color(0, 0, 0);
    if (objColor == "Black") { RGBColor = strip.Color(0, 0, 0); }
    if (objColor == "White") { RGBColor = strip.Color(255, 255, 255); }
    if (objColor == "Red") { RGBColor = strip.Color(255, 0, 0); }
    if (objColor == "Lime") { RGBColor = strip.Color(0, 255, 0); }
    if (objColor == "Blue") { RGBColor = strip.Color(0, 0, 255); }
    if (objColor == "Yellow") { RGBColor = strip.Color(255, 255, 0); }
    if (objColor == "Cyan/Aqua") { RGBColor = strip.Color(0, 255, 255); }
    if (objColor == "Magenta/Fuchsia") { RGBColor = strip.Color(255, 0, 255); }
    if (objColor == "Silver") { RGBColor = strip.Color(192, 192, 192); }
    if (objColor == "Gray") { RGBColor = strip.Color(128, 128, 128); }
    //if (objColor == "Maroon") { RGBColor = strip.Color(128, 0, 0); }
    //if (objColor == "Olive") { RGBColor = strip.Color(128, 128, 0); }
    if (objColor == "Green") { RGBColor = strip.Color(0, 128, 0); }
    if (objColor == "Purple") { RGBColor = strip.Color(128, 0, 128); }
    //if (objColor == "Teal") { RGBColor = strip.Color(0, 128, 128); }
    //if (objColor == "Navy") { RGBColor = strip.Color(0, 0, 128); }

    /*if (objColor == "maroon") { RGBColor = strip.Color(128, 0, 0); }
    if (objColor == "dark red") { RGBColor = strip.Color(139, 0, 0); }
    if (objColor == "brown") { RGBColor = strip.Color(165, 42, 42); }
    if (objColor == "firebrick") { RGBColor = strip.Color(178, 34, 34); }
    if (objColor == "crimson") { RGBColor = strip.Color(220, 20, 60); }
    if (objColor == "red") { RGBColor = strip.Color(255, 0, 0); }
    if (objColor == "tomato") { RGBColor = strip.Color(255, 99, 71); }
    if (objColor == "coral") { RGBColor = strip.Color(255, 127, 80); }
    if (objColor == "indian red") { RGBColor = strip.Color(205, 92, 92); }
    if (objColor == "light coral") { RGBColor = strip.Color(240, 128, 128); }
    if (objColor == "dark salmon") { RGBColor = strip.Color(233, 150, 122); }
    if (objColor == "salmon") { RGBColor = strip.Color(250, 128, 114); }
    if (objColor == "light salmon") { RGBColor = strip.Color(255, 160, 122); }
    if (objColor == "orange red") { RGBColor = strip.Color(255, 69, 0); }
    if (objColor == "dark orange") { RGBColor = strip.Color(255, 140, 0); }*/
    if (objColor == "orange") { RGBColor = strip.Color(255, 165, 0); }
    /*if (objColor == "gold") { RGBColor = strip.Color(255, 215, 0); }
    if (objColor == "dark golden rod") { RGBColor = strip.Color(184, 134, 11); }
    if (objColor == "golden rod") { RGBColor = strip.Color(218, 165, 32); }
    if (objColor == "pale golden rod") { RGBColor = strip.Color(238, 232, 170); }
    if (objColor == "dark khaki") { RGBColor = strip.Color(189, 183, 107); }
    if (objColor == "khaki") { RGBColor = strip.Color(240, 230, 140); }
    if (objColor == "olive") { RGBColor = strip.Color(128, 128, 0); }
    if (objColor == "yellow") { RGBColor = strip.Color(255, 255, 0); }
    if (objColor == "yellow green") { RGBColor = strip.Color(154, 205, 50); }
    if (objColor == "dark olive green") { RGBColor = strip.Color(85, 107, 47); }
    if (objColor == "olive drab") { RGBColor = strip.Color(107, 142, 35); }
    if (objColor == "lawn green") { RGBColor = strip.Color(124, 252, 0); }
    if (objColor == "chartreuse") { RGBColor = strip.Color(127, 255, 0); }
    if (objColor == "green yellow") { RGBColor = strip.Color(173, 255, 47); }
    if (objColor == "dark green") { RGBColor = strip.Color(0, 100, 0); }
    if (objColor == "green") { RGBColor = strip.Color(0, 128, 0); }
    if (objColor == "forest green") { RGBColor = strip.Color(34, 139, 34); }
    if (objColor == "lime") { RGBColor = strip.Color(0, 255, 0); }
    if (objColor == "lime green") { RGBColor = strip.Color(50, 205, 50); }
    if (objColor == "light green") { RGBColor = strip.Color(144, 238, 144); }
    if (objColor == "pale green") { RGBColor = strip.Color(152, 251, 152); }
    if (objColor == "dark sea green") { RGBColor = strip.Color(143, 188, 143); }
    if (objColor == "medium spring green") { RGBColor = strip.Color(0, 250, 154); }
    if (objColor == "spring green") { RGBColor = strip.Color(0, 255, 127); }
    if (objColor == "sea green") { RGBColor = strip.Color(46, 139, 87); }
    if (objColor == "medium aqua marine") { RGBColor = strip.Color(102, 205, 170); }
    if (objColor == "medium sea green") { RGBColor = strip.Color(60, 179, 113); }
    if (objColor == "light sea green") { RGBColor = strip.Color(32, 178, 170); }
    if (objColor == "dark slate gray") { RGBColor = strip.Color(47, 79, 79); }
    if (objColor == "teal") { RGBColor = strip.Color(0, 128, 128); }
    if (objColor == "dark cyan") { RGBColor = strip.Color(0, 139, 139); }
    if (objColor == "aqua") { RGBColor = strip.Color(0, 255, 255); }
    if (objColor == "cyan") { RGBColor = strip.Color(0, 255, 255); }
    if (objColor == "light cyan") { RGBColor = strip.Color(224, 255, 255); }
    if (objColor == "dark turquoise") { RGBColor = strip.Color(0, 206, 209); }
    if (objColor == "turquoise") { RGBColor = strip.Color(64, 224, 208); }
    if (objColor == "medium turquoise") { RGBColor = strip.Color(72, 209, 204); }
    if (objColor == "pale turquoise") { RGBColor = strip.Color(175, 238, 238); }
    if (objColor == "aqua marine") { RGBColor = strip.Color(127, 255, 212); }
    if (objColor == "powder blue") { RGBColor = strip.Color(176, 224, 230); }
    if (objColor == "cadet blue") { RGBColor = strip.Color(95, 158, 160); }
    if (objColor == "steel blue") { RGBColor = strip.Color(70, 130, 180); }
    if (objColor == "corn flower blue") { RGBColor = strip.Color(100, 149, 237); }
    if (objColor == "deep sky blue") { RGBColor = strip.Color(0, 191, 255); }
    if (objColor == "dodger blue") { RGBColor = strip.Color(30, 144, 255); }
    if (objColor == "light blue") { RGBColor = strip.Color(173, 216, 230); }
    if (objColor == "sky blue") { RGBColor = strip.Color(135, 206, 235); }
    if (objColor == "light sky blue") { RGBColor = strip.Color(135, 206, 250); }
    if (objColor == "midnight blue") { RGBColor = strip.Color(25, 25, 112); }
    if (objColor == "navy") { RGBColor = strip.Color(0, 0, 128); }
    if (objColor == "dark blue") { RGBColor = strip.Color(0, 0, 139); }
    if (objColor == "medium blue") { RGBColor = strip.Color(0, 0, 205); }
    if (objColor == "blue") { RGBColor = strip.Color(0, 0, 255); }
    if (objColor == "royal blue") { RGBColor = strip.Color(65, 105, 225); }
    if (objColor == "blue violet") { RGBColor = strip.Color(138, 43, 226); }
    if (objColor == "indigo") { RGBColor = strip.Color(75, 0, 130); }
    if (objColor == "dark slate blue") { RGBColor = strip.Color(72, 61, 139); }
    if (objColor == "slate blue") { RGBColor = strip.Color(106, 90, 205); }
    if (objColor == "medium slate blue") { RGBColor = strip.Color(123, 104, 238); }
    if (objColor == "medium purple") { RGBColor = strip.Color(147, 112, 219); }
    if (objColor == "dark magenta") { RGBColor = strip.Color(139, 0, 139); }
    if (objColor == "dark violet") { RGBColor = strip.Color(148, 0, 211); }
    if (objColor == "dark orchid") { RGBColor = strip.Color(153, 50, 204); }
    if (objColor == "medium orchid") { RGBColor = strip.Color(186, 85, 211); }
    if (objColor == "purple") { RGBColor = strip.Color(128, 0, 128); }
    if (objColor == "thistle") { RGBColor = strip.Color(216, 191, 216); }
    if (objColor == "plum") { RGBColor = strip.Color(221, 160, 221); }
    if (objColor == "violet") { RGBColor = strip.Color(238, 130, 238); }
    if (objColor == "magenta / fuchsia") { RGBColor = strip.Color(255, 0, 255); }
    if (objColor == "orchid") { RGBColor = strip.Color(218, 112, 214); }
    if (objColor == "medium violet red") { RGBColor = strip.Color(199, 21, 133); }
    if (objColor == "pale violet red") { RGBColor = strip.Color(219, 112, 147); }
    if (objColor == "deep pink") { RGBColor = strip.Color(255, 20, 147); }
    if (objColor == "hot pink") { RGBColor = strip.Color(255, 105, 180); }
    if (objColor == "light pink") { RGBColor = strip.Color(255, 182, 193); }
    if (objColor == "pink") { RGBColor = strip.Color(255, 192, 203); }
    if (objColor == "antique white") { RGBColor = strip.Color(250, 235, 215); }
    if (objColor == "beige") { RGBColor = strip.Color(245, 245, 220); }
    if (objColor == "bisque") { RGBColor = strip.Color(255, 228, 196); }
    if (objColor == "blanched almond") { RGBColor = strip.Color(255, 235, 205); }
    if (objColor == "wheat") { RGBColor = strip.Color(245, 222, 179); }
    if (objColor == "corn silk") { RGBColor = strip.Color(255, 248, 220); }
    if (objColor == "lemon chiffon") { RGBColor = strip.Color(255, 250, 205); }
    if (objColor == "light golden rod yellow") { RGBColor = strip.Color(250, 250, 210); }
    if (objColor == "light yellow") { RGBColor = strip.Color(255, 255, 224); }
    if (objColor == "saddle brown") { RGBColor = strip.Color(139, 69, 19); }
    if (objColor == "sienna") { RGBColor = strip.Color(160, 82, 45); }
    if (objColor == "chocolate") { RGBColor = strip.Color(210, 105, 30); }
    if (objColor == "peru") { RGBColor = strip.Color(205, 133, 63); }
    if (objColor == "sandy brown") { RGBColor = strip.Color(244, 164, 96); }
    if (objColor == "burly wood") { RGBColor = strip.Color(222, 184, 135); }
    if (objColor == "tan") { RGBColor = strip.Color(210, 180, 140); }
    if (objColor == "rosy brown") { RGBColor = strip.Color(188, 143, 143); }
    if (objColor == "moccasin") { RGBColor = strip.Color(255, 228, 181); }
    if (objColor == "navajo white") { RGBColor = strip.Color(255, 222, 173); }
    if (objColor == "peach puff") { RGBColor = strip.Color(255, 218, 185); }
    if (objColor == "misty rose") { RGBColor = strip.Color(255, 228, 225); }
    if (objColor == "lavender blush") { RGBColor = strip.Color(255, 240, 245); }
    if (objColor == "linen") { RGBColor = strip.Color(250, 240, 230); }
    if (objColor == "old lace") { RGBColor = strip.Color(253, 245, 230); }
    if (objColor == "papaya whip") { RGBColor = strip.Color(255, 239, 213); }
    if (objColor == "sea shell") { RGBColor = strip.Color(255, 245, 238); }
    if (objColor == "mint cream") { RGBColor = strip.Color(245, 255, 250); }
    if (objColor == "slate gray") { RGBColor = strip.Color(112, 128, 144); }
    if (objColor == "light slate gray") { RGBColor = strip.Color(119, 136, 153); }
    if (objColor == "light steel blue") { RGBColor = strip.Color(176, 196, 222); }
    if (objColor == "lavender") { RGBColor = strip.Color(230, 230, 250); }
    if (objColor == "floral white") { RGBColor = strip.Color(255, 250, 240); }
    if (objColor == "alice blue") { RGBColor = strip.Color(240, 248, 255); }
    if (objColor == "ghost white") { RGBColor = strip.Color(248, 248, 255); }
    if (objColor == "honeydew") { RGBColor = strip.Color(240, 255, 240); }
    if (objColor == "ivory") { RGBColor = strip.Color(255, 255, 240); }
    if (objColor == "azure") { RGBColor = strip.Color(240, 255, 255); }
    if (objColor == "snow") { RGBColor = strip.Color(255, 250, 250); }
    if (objColor == "black") { RGBColor = strip.Color(0, 0, 0); }
    if (objColor == "dim gray / dim grey") { RGBColor = strip.Color(105, 105, 105); }
    if (objColor == "gray / grey") { RGBColor = strip.Color(128, 128, 128); }
    if (objColor == "dark gray / dark grey") { RGBColor = strip.Color(169, 169, 169); }
    if (objColor == "silver") { RGBColor = strip.Color(192, 192, 192); }
    if (objColor == "light gray / light grey") { RGBColor = strip.Color(211, 211, 211); }
    if (objColor == "gainsboro") { RGBColor = strip.Color(220, 220, 220); }
    if (objColor == "white smoke") { RGBColor = strip.Color(245, 245, 245); }
    if (objColor == "white") { RGBColor = strip.Color(255, 255, 255); }
    if (objColor == "white smoke ") { RGBColor = strip.Color(245, 245, 245); }
    if (objColor == "white ") { RGBColor = strip.Color(255, 255, 255); }*/

    return RGBColor;
}