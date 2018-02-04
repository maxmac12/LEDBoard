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

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

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
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 2,  NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 14, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 7,  NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 8,  NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 6,  NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 20, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 21, NEO_GRB + NEO_KHZ800),
    new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 5,  NEO_GRB + NEO_KHZ800)
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
    currentColor(0x00000000)  // Off
{
    // Create each LED mode state machine.
    ptrLedStateMachines[COLOR]              = new SMColor();
    ptrLedStateMachines[COLOR_PULSE]        = new SMColorPulse();
    ptrLedStateMachines[RAINBOW_CYCLE]      = new SMRainbowCycle();
    ptrLedStateMachines[WHITE_OVER_RAINBOW] = new SMWhiteOverRainbow();
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

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

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
