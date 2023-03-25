/*
 Name:		RGB_led_strip_OctoPrint.ino
 Created:	21/03/2023
 Author:	A.P. Ebbers
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN 13
#define NUM_LEDS 47  // Number of LEDs in strip
String message = ""; // Declare the message outside any void. Otherwise the message cannot be increased with lines 25.

// ------------Define the LED strip based on above declarations--------------
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
#define DELAYVAL 0 // time to go from one LED to the other.

// ------------For an push button attached to the arduino-------------------
const int buttonPin = 8; // the number of the pushbutton pin
int buttonState = 0;     // variable for reading the pushbutton status

// the setup function runs once when you press reset or power the board
void setup()
{
    delay(3000); // power-up safety delay

    strip.begin(); // Initialize NeoPixel strip object (REQUIRED)

    Serial.begin(9600); // Make the connection with bautrate 9600

    pinMode(buttonPin, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop()
{

    // Code to turn on the ligth manually--------------------------------------------------------------
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH and enable the LEDstrip:
    if (buttonState == HIGH)
    {
        EnableLEDStrip("Warm white");
    }
    //------------------------------------------------------------------------------------------------

    // Code for the OctoPrint plugin------------------------------------------------------------------
    while (Serial.available() > 0)
    {
        int inByte;
        inByte = Serial.read();
        // Serial.println(inByte); // Print the message for debugging

        message = message + char(inByte); // Add the incoming byte to our message

        //-----------------------------------------------------------------------
        // Mandetory part of the code. This is needed to let the OctoPrint plugin add your device to
        // the list of available devices
        // If asked for ID, return the ID
        if (message.endsWith("GetID"))
        {
            // Return ID. Define your own. Don't forget to add '\n' at the end!!!
            Serial.write("Arduino #1\n");
        }
        //-----------------------------------------------------------------------

        // Code for the On/Off switch on the navbar in octoprint-----------------
        if (message.endsWith("SwitchOn"))
        {
            EnableLEDStrip("Warm white");
        }

        if (message.endsWith("SwitchOff"))
        {
            EnableLEDStrip("Black");
        }
        //-----------------------------------------------------------------------

        if (message.endsWith("M500"))
        {
            EnableLEDStrip("Purple");
            delay(5000);
            EnableLEDStrip("Warm white");
        }

        if (message.endsWith("M503"))
        {
            EnableLEDStrip("Orange");
            delay(5000);
            EnableLEDStrip("Warm white");
        }

        if (message.endsWith("Upload"))
        {
            EnableLEDStrip("Green");
            delay(5000);
            EnableLEDStrip("Warm white");
        }

        if (message.endsWith("SettingsUpdated"))
        {
            EnableLEDStrip("Green");
            delay(5000);
            EnableLEDStrip("Warm white");
        }

        if (message.endsWith("Finished"))
        {
            EnableLEDStrip("Green");
        }

        if (message.endsWith("Started") || message.endsWith("any"))
        {
            EnableLEDStrip("Warm white");
        }

        if (message.endsWith("PrintCancelled") || message.endsWith("cancel"))
        {
            EnableLEDStrip("Red");
            delay(5000);
            EnableLEDStrip("Warm white");
        }

        if (message.endsWith("PrintPaused") || message.endsWith("paused"))
        {
            EnableLEDStrip("Orange");
        }

        if (message.endsWith("Disconnect") || message.endsWith("error"))
        {
            EnableLEDStrip("Red");
        }

        if (message.endsWith("Connected"))
        {
            EnableLEDStrip("Warm white");
        }
    }
    //------------------------------------------------------------------------------------------------
}

void EnableLEDStrip(String objColor)
{
    uint32_t objIndexColor = IndexColor(objColor);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        strip.setPixelColor(i, IndexColor(objColor)); // Set the color
        strip.show();                                 // Send the updated pixel colors to the hardware.
        delay(DELAYVAL);                              // Pause before next pass through loop
    }
}
// Disable the colors you don't need to save memory.
uint32_t IndexColor(String objColor)
{
    uint32_t RGBColor = strip.Color(0, 0, 0);
    if (objColor == "Black")
    {
        RGBColor = strip.Color(0, 0, 0);
    }
    if (objColor == "White")
    {
        RGBColor = strip.Color(255, 255, 255);
    }
    if (objColor == "Red")
    {
        RGBColor = strip.Color(255, 0, 0);
    }
    if (objColor == "Blue")
    {
        RGBColor = strip.Color(0, 0, 255);
    }
    if (objColor == "Yellow")
    {
        RGBColor = strip.Color(255, 255, 0);
    }
    if (objColor == "Cyan/Aqua")
    {
        RGBColor = strip.Color(0, 255, 255);
    }
    if (objColor == "Magenta/Fuchsia")
    {
        RGBColor = strip.Color(255, 0, 255);
    }
    if (objColor == "Gray")
    {
        RGBColor = strip.Color(128, 128, 128);
    }
    if (objColor == "Green")
    {
        RGBColor = strip.Color(0, 128, 0);
    }
    if (objColor == "Purple")
    {
        RGBColor = strip.Color(128, 0, 128);
    }
    if (objColor == "Orange")
    {
        RGBColor = strip.Color(255, 69, 0);
    }

    if (objColor == "Warm white")
    {
        RGBColor = strip.Color(244, 164, 96);
    }

    return RGBColor;
}