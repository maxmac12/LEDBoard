//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "LEDControl.hpp"
#include "Adafruit_NeoPixel.h"
#include "appconfig.hpp"
#include "StateMachine.hpp"
#include "SMColor.hpp"
#include "SMColorPulse.hpp"
#include "SMRainbowCycle.hpp"
#include "SMWhiteOverRainbow.hpp"

#include "MaintenanceComm.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------
#define MOM_SW_PIN       A0  // Momentary switch pin.
#define MOM_SW_DEBOUNCE  40  // Debounce time (Msec) for momentary switch.

#define BRIGHTNESS_PIN        A1  // Brightness potentiometer pin.
#define BRIGHTNESS_TOLERANCE  2   // +/- tolerance of change for brightness.
#define BRIGHTNESS_FLOOR      5   // Set brightness to minimum when less than this value.

#define ANALOG_READ_DELAY  60  // TODO: Update to get a better ADC average (128Hz, 32Hz, ...)

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

// Define and create all of the LED strips in the system.
Adafruit_NeoPixel* strips[NUM_LED_STRIPS] =
{
    // Add additional LED strips here.
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 2, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 3, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 4, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 5, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 6, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 7, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 8, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 9, NEO_GRB + NEO_KHZ800)
};

//############################################################################
//  Public Methods
//############################################################################

LEDControl::LEDControl() :
    Task(TASK_LED_CTRL_PERIOD,
         TASK_LED_CTRL_NAME,
         TID_LED_CTRL),
    currentMode(COLOR),
    previousMode(COLOR),
    currentBrightness(MAX_LED_BRIGHTNESS),
    currentColor(0x00000000),  // Off
    ptrBrightnessTimer(new StopWatch()),
    ptrMomSwitchDebounce(new StopWatch())
{
    // Create each LED mode state machine.
    ptrLedStateMachines[COLOR]              = new SMColor();
    ptrLedStateMachines[COLOR_PULSE]        = new SMColorPulse();
    ptrLedStateMachines[RAINBOW_CYCLE]      = new SMRainbowCycle();
    ptrLedStateMachines[WHITE_OVER_RAINBOW] = new SMWhiteOverRainbow();

    ptrBrightnessTimer->start(ANALOG_READ_DELAY);
}


void LEDControl::init(void)
{
    // Initialize all pixels.
    for (U32 i = 0; i < NUM_LED_STRIPS; i++)
    {
        strips[i]->setBrightness(currentBrightness);
        strips[i]->begin();
        strips[i]->show();
    }

    // Initialize all LED state machines.
    for (U32 i = 0; i < NUM_LED_MODES; i++)
    {
        ptrLedStateMachines[i]->init();
    }

    // Configure momentary switch pin.
    pinMode(MOM_SW_PIN, INPUT_PULLUP);
}


void LEDControl::exec(void)
{
    // Check if the LED mode has changed.
    if (currentMode != previousMode)
    {
        // State changed. Reset the current active
        ptrLedStateMachines[previousMode]->reset();
        previousMode = currentMode;
    }

    // Run the active LED mode state machine.
    ptrLedStateMachines[currentMode]->run();

    // Read brightness ADC input.
    readAnalogBrightness();

    // Read the momentary switch input.
    readMomentarySwitch();
}


LEDModes LEDControl::getLedMode(void)
{
    return currentMode;
}


void LEDControl::setLedMode(LEDModes newMode)
{
    currentMode = newMode;
}


void LEDControl::setBrightness(U8 brightness)
{
    if (brightness >= MAX_LED_BRIGHTNESS)
    {
        currentBrightness = MAX_LED_BRIGHTNESS;
    }
    else if (brightness <= MIN_LED_BRIGHTNESS)
    {
        currentBrightness = MIN_LED_BRIGHTNESS;
    }
    else
    {
        currentBrightness = brightness;
    }

    for (S32 i = 0; i < NUM_LED_STRIPS; i++)
    {
        strips[i]->setBrightness(currentBrightness);
        strips[i]->show();
    }
}


void LEDControl::setWhiteRainbowLength(U32 length)
{
    // TODO: Create a better interface.
    reinterpret_cast<SMWhiteOverRainbow*>(ptrLedStateMachines[WHITE_OVER_RAINBOW])->setWhiteLength(length);
}


void LEDControl::setWhiteRainbowSpeed(Msec speed)
{
    // TODO: Create a better interface.
        reinterpret_cast<SMWhiteOverRainbow*>(ptrLedStateMachines[WHITE_OVER_RAINBOW])->setWhiteSpeed(speed);
}


void LEDControl::setPulseSpeed(Msec speed)
{
    // TODO: Create a better interface.
    reinterpret_cast<SMColorPulse*>(ptrLedStateMachines[COLOR_PULSE])->setPulseSpeed(speed);
}


U8 LEDControl::getRedChannel(U32 color)
{
    return (color >> 16);
}


U8 LEDControl::getGreenChannel(U32 color)
{
    return (color >> 8);
}


U8 LEDControl::getBlueChannel(U32 color)
{
    return (color);
}


// Convert separate R,G,B into packed 32-bit RGB color.
U32 LEDControl::getColorValue(U8 r, U8 g, U8 b)
{
    return ((U32)r << 16) | ((U32)g <<  8) | b;
}


// Convert 32-bit color into R,G,B channels.
void LEDControl::getColorChannels(U32 color, U8& r, U8& g, U8& b)
{
    r = getRedChannel(color);
    g = getGreenChannel(color);
    b = getBlueChannel(color);
}


U32 LEDControl::getCurrentColor(void)
{
    return currentColor;
}


void LEDControl::setCurrentColor(U8 c)
{
    currentColor = c;
}


void LEDControl::setCurrentColor(U8 r, U8 g, U8 b)
{
    currentColor = getColorValue(r, g, b);
}


U32 LEDControl::getNumPixels(U32 stripId)
{
    return (stripId < NUM_LED_STRIPS) ? strips[stripId]->numPixels() : 0;
}


void LEDControl::setPixelColor(U32 stripId, U32 pixelId, U32 color)
{
    if (stripId < NUM_LED_STRIPS)
    {
        if (pixelId < strips[stripId]->numPixels())
        {
            strips[stripId]->setPixelColor(pixelId, color);
        }
    }

}


void LEDControl::updateStrip(U32 stripId)
{
    if (stripId < NUM_LED_STRIPS)
    {
        strips[stripId]->show();
    }
}


void LEDControl::updateAllStrips(void)
{
    for (U32 i = 0; i < NUM_LED_STRIPS; i++)
    {
        strips[i]->show();
    }
}


// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
U32 LEDControl::Wheel(S8 WheelPos, U32 stripId)
{
    WheelPos = 255 - WheelPos;

    if (WheelPos < 85)
    {
        return strips[stripId]->Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
    }

    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strips[stripId]->Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
    }

    WheelPos -= 170;
    return strips[stripId]->Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}


void LEDControl::turnLedsOff(void)
{
    for (S32 i = 0; i < NUM_LED_STRIPS; i++)
    {
        U16 numPixels = ledCtrl->getNumPixels(i);

        for (U16 j = 0; j < numPixels; j++)
        {
            ledCtrl->setPixelColor(i, j, 0x00000000);
        }

        ledCtrl->updateStrip(i);
    }
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################


void LEDControl::readAnalogBrightness(void)
{
    // TODO: Implement an IIR filter for the ADC values. Currently just an average over 4 readings.
    // TODO: Scale the readings to the MAX_BRIGHTNESS.
    const U8 MAX_ADC_READS = 4;
    static U8 numReadings = 0;
    static U32 brightAdcTotal;

    if (ptrBrightnessTimer->timerHasExpired())
    {
        // ADC values are 10-bit (0 - 1023)
        // Brightness values are between 0 - 255.
        // Divide the ADC value by 4 to get a brightness value.
        brightAdcTotal += (U32)analogRead(BRIGHTNESS_PIN);

        if (++numReadings >= MAX_ADC_READS)
        {
            // Get the average of the readings by dividing by 4 and
            // scale readings down to 8-bit brightness value.
            brightAdcTotal >>= 4;

            // Color Pulse state machine controls the brightness when running.
            // Add +/- 4 tolerance to ADC average.
            if ((currentMode != COLOR_PULSE) &&
               ((brightAdcTotal >= (currentBrightness + BRIGHTNESS_TOLERANCE)) ||
                (brightAdcTotal <= (currentBrightness - BRIGHTNESS_TOLERANCE))))
            {
                if (brightAdcTotal < BRIGHTNESS_FLOOR)
                {
                    // Turn LEDs OFF.
                    turnLedsOff();
                }
                else
                {
                    // Set the brightness based on the ADC reading.
                    setBrightness(brightAdcTotal);
                }
            }

            // Reset ADC readings.
            numReadings = 0;
            brightAdcTotal = 0;
        }

        ptrBrightnessTimer->start(ANALOG_READ_DELAY);
    }

}


void LEDControl::readMomentarySwitch(void)
{
    static bool debounceInProgress = false;  // Flag indicating if the switch is being debounced.
    static U8 lastSwState          = HIGH;   // Momentary switch is GND-Active.

    U8 currSwState = digitalRead(MOM_SW_PIN);

    // Check if switch state has changed.
    if (currSwState != lastSwState)
    {
        // Switch state has changed. Begin debounce.
        ptrMomSwitchDebounce->start(MOM_SW_DEBOUNCE);
        debounceInProgress = true;
    }

    // Check if the switch has been debounced.
    if ((ptrMomSwitchDebounce->timerHasExpired()) &&
        (debounceInProgress))
    {
        // Switch debounce complete.
        debounceInProgress = false;

        // Switch state has been debounced.
        if (LOW == currSwState)
        {
            // Switch is active. Cycle through the different LED modes.
            if ((LEDModes)(currentMode + 1) >= NUM_LED_MODES)
            {
                // Rollover to the beginning of the LED mode list.
                setLedMode((LEDModes)(0));
            }
            else
            {
                // Increment to the next LED mode.
                setLedMode((LEDModes)(currentMode + 1));
            }
        }
    }

    lastSwState = currSwState;
}


// TODO: Determine if this function can be deleted.
void LEDControl::rainbow(U8 wait)
{
    for (U16 j = 0; j < 256; j++)
    {
        for (S32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (U16 i = 0; i < strips[n]->numPixels(); i++)
            {
                strips[n]->setPixelColor(i, Wheel((i+j) & 255, n));
            }
        }

        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n]->show();
            delay(wait);
        }
    }
}
