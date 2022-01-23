/*
 Name:		RGB_led_strip_OctoPrint.ino
 Created:	1/14/2022 8:55:02 PM
 Author:	SKIKK
*/

#include <SoftwareSerial.h>
#include <FastLED.h>
#include <EEPROM.h>

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

#define LED_PIN     13
#define NUM_LEDS    11
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
String message = ""; // Declare the message outside any void. Otherwise the message cannot be increased with lines 25. 

// the setup function runs once when you press reset or power the board
void setup() {    
    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = myRedWhiteBluePalette_p;
    currentBlending = LINEARBLEND;

    FastLED.showColor(CRGB::Gray);

    Serial.begin(9600); // Make the connection with bautrate 9600
}

// the loop function runs over and over again until power down or reset
void loop() {
    while (Serial.available() > 0)
    {
        char inByte;
        inByte = Serial.read(); // Print the message for debugging
        //Serial.println(inByte);

        message = message + inByte; // Add the incoming byte to our message

        if (message.endsWith("M500"))
        {
            message + '\0'; // Add null character to string
            Serial.println(message); // Print the message for debugging

            FastLED.showColor(CRGB::Purple);

            message = ""; //Clear the message for the next message in the loop           
            if (message == "") Serial.println("message is cleared!");  //If the message is cleared, print this message for debugging.
        }

        if (message.endsWith("M503"))
        {
            message + '\0'; // Add null character to string
            Serial.println(message); // Print the message for debugging

            FastLED.showColor(CRGB::Yellow);

            message = ""; //Clear the message for the next message in the loop           
            if (message == "") Serial.println("message is cleared!");  //If the message is cleared, print this message for debugging.
        }

        if (message.endsWith("SettingsUpdated") || message.endsWith("Finished"))
        {
            message + '\0'; // Add null character to string
            Serial.println(message); // Print the message for debugging

            FastLED.showColor(CRGB::Green);

            message = ""; //Clear the message for the next message in the loop           
            if (message == "") Serial.println("message is cleared!");  //If the message is cleared, print this message for debugging.
        }

        if (message.endsWith("Started") || message.endsWith("any"))
        {
            message + '\0'; // Add null character to string
            Serial.println(message); // Print the message for debugging

            FastLED.showColor(CRGB::Gray);

            message = ""; //Clear the message for the next message in the loop           
            if (message == "") Serial.println("message is cleared!");  //If the message is cleared, print this message for debugging.
        }

        if (message.endsWith("PrintCancelled") || message.endsWith("any")|| message.endsWith("cancel"))
        {
            message + '\0'; // Add null character to string
            Serial.println(message); // Print the message for debugging

            FastLED.showColor(CRGB::Red);

            message = ""; //Clear the message for the next message in the loop           
            if (message == "") Serial.println("message is cleared!");  //If the message is cleared, print this message for debugging.
        }

        if (message.endsWith("PrintPaused") || message.endsWith("paused") || message.endsWith("disconnect"))
        {
            message + '\0'; // Add null character to string
            Serial.println(message); // Print the message for debugging

            FastLED.showColor(CRGB::Orange);

            message = ""; //Clear the message for the next message in the loop           
            if (message == "") Serial.println("message is cleared!");  //If the message is cleared, print this message for debugging.
        }
    }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if (lastSecond != secondHand) {
        lastSecond = secondHand;
        if (secondHand == 0)
            /*{ currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }*/
        {
            currentPalette = myRedWhiteBluePalette_p;
            currentBlending = LINEARBLEND;
        }
        /*if (secondHand == 10) { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND; }
        if (secondHand == 15) { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if (secondHand == 20) { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if (secondHand == 25) { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if (secondHand == 30) { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if (secondHand == 35) { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if (secondHand == 40) { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if (secondHand == 45) { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if (secondHand == 50) { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND; }
        if (secondHand == 55) { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }*/
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for (int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    currentPalette = CRGBPalette16(
        green, green, black, black,
        purple, purple, black, black,
        green, green, black, black,
        purple, purple, black, black);
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,

 /*   CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black*/
};

// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.