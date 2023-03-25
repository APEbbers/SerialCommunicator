#include <EEPROM.h>

int led = 13;    // The pin the LED is connected to
int LED_STATE_1; // The holder for the value in EEPROM
int LED_VALUE;
String message = ""; // Declare the message outside any void. Otherwise the message cannot be increased with lines 25.

void setup()
{
    Serial.begin(9600);   // Make the connection with bautrate 9600
    pinMode(led, OUTPUT); // Declare the LED as an output

    CheckLEDState(); // Switch the LED on or off at start. Depening last saved state in EEPROM.
}

void loop()
{
    // Check to see if anything is available in the serial receive buffer
    while (Serial.available() > 0)
    {
        char inByte;
        inByte = Serial.read(); // Print the message for debugging
        Serial.println(inByte);

        message = message + inByte; // Add the incoming byte to our message
        Serial.println(message);    // Print the message for debugging

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

        // If the end of the message is M500 or M503 then make the LED to switch on and of 10 times.
        if (message.endsWith("M500") || message.endsWith("M503") || message.endsWith("SettingsUpdated") || message.endsWith("PrintStarted") || message.endsWith("PrintPaused"))
        {
            message + '\0';          // Add null character to string
            Serial.println(message); // Print the message for debugging

            // FLicker the LED 10 times. This will NOT be written to the EEPROM.
            FlickerLED(200, 10);

            message = ""; // Clear the message for the next message in the loop
            if (message == "")
                Serial.println("message is cleared!"); // If the message is cleared, print this message for debugging.
        }

        // If the end of the message is M500 or M503 then make the LED to switch on and of 10 times.
        if (message.endsWith("notification"))
        {
            message + '\0';          // Add null character to string
            Serial.println(message); // Print the message for debugging

            // FLicker the LED 10 times. This will NOT be written to the EEPROM.
            FlickerLED(100, 20);

            message = ""; // Clear the message for the next message in the loop
            if (message == "")
                Serial.println("message is cleared!"); // If the message is cleared, print this message for debugging.
        }

        // If the end of the message is OCTO1 then switch the LED ON
        if (message.endsWith("OCTO1"))
        {
            message + '\0';          // Add null character to string
            Serial.println(message); // Print the message for debugging

            EnableLED(true); // Change the state of the LED. This function writes the state to the EEPROM of the arduino.

            message = ""; // Clear the message for the next message in the loop
            if (message == "")
                Serial.println("message is cleared!"); // If the message is cleared, print this message for debugging.
        }

        // If the end of the message is OCTO2 then switch the LED OFF
        if (message.endsWith("OCTO2"))
        {
            message + '\0';          // Add null character to string
            Serial.println(message); // Print the message for debugging

            EnableLED(false); // Change the state of the LED. This function writes the state to the EEPROM of the arduino.

            message = ""; // Clear the message for the next message in the loop
            if (message == "")
                Serial.println("message is cleared!"); // If the message is cleared, print this message for debugging.
        }
    }
}

// Function to switch a LED on or off.
// Write the state to the EEPROM to load the last state of the LED when powering the arduino next time.
void EnableLED(bool Enabled)
{
    if (Enabled == true)
    {
        LED_STATE_1 = 1;
        digitalWrite(led, HIGH);       // Switch the LED on.
        EEPROM.update(0, LED_STATE_1); // Write the state of the LED to EEPROM in slot 0.
    }
    if (Enabled == false)
    {
        LED_STATE_1 = 0;
        digitalWrite(led, LOW);        // Switch the LED off.
        EEPROM.update(0, LED_STATE_1); // Write the state of the LED to EEPROM in slot 0.
    }
    Serial.println(EEPROM.read(0)); // Print the EEPROM value for debugging
}

// Read the EEPROM value and set the state of the LED accordingly.
void CheckLEDState()
{
    LED_STATE_1 = EEPROM.read(0); // READ the EEPROM value in slot 0.
    if (LED_STATE_1 == 1)
        digitalWrite(led, HIGH); // Switch the LED on.
    if (LED_STATE_1 == 0)
        digitalWrite(led, LOW); // Switch the LED off.

    Serial.println(EEPROM.read(0)); // Print the EEPROM value for debugging
}

// FLickers the LED. Afterwards switch to the last saved state.
void FlickerLED(int Delay, int NoOfTimes)
{
    int x = 0;
    do
    {
        x = x + 1;
        digitalWrite(led, HIGH);
        delay(Delay);
        digitalWrite(led, LOW);
        delay(Delay);
    } while (x < NoOfTimes);

    CheckLEDState(); // Switch the LED on or off. Depening last saved state in EEPROM.
}
