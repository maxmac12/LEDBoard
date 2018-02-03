//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "LEDControl.hpp"
#include "Adafruit_NeoPixel.h"
#include "appconfig.hpp"
#include "StateMachine.hpp"
#include "SMRainbowCycle.hpp"
#include "SMWhiteOverRainbow.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

// TODO: Refactor to be a brightness slope instead of a WHITE slope.
const S8 neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

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

StateMachine* ptrRainbow      = new SMRainbowCycle();  // TODO: Create an array of state machines.
StateMachine* ptrWhiteRainbow = new SMWhiteOverRainbow();

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

//############################################################################
//  Public Methods
//############################################################################

LEDControl::LEDControl() :
    Task(TASK_LED_CTRL_PERIOD,
         TASK_LED_CTRL_NAME,
         TID_LED_CTRL),
    currentMode(IDLE),
    previousMode(IDLE),
    currentBrightness(MAX_LED_BRIGHTNESS),
    currentColor(0)  // Off
{
    // Do nothing.
}


void LEDControl::init(void)
{
    // Initialize all pixels to 'off'
    for (S32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n]->setBrightness(currentBrightness);
        strips[n]->begin();
        strips[n]->show();
    }

    ptrRainbow->init();
    ptrWhiteRainbow->init();
}


void LEDControl::exec(void)
{
    if (currentMode != previousMode)
    {
        // TODO: Add additional state machines.
        // Reset all state machines.
        ptrRainbow->reset();
        ptrWhiteRainbow->reset();
        previousMode = currentMode;
    }

    // TODO: Break each state up to minimize time in this task. Only update one strip per frame?
    switch (currentMode)
    {
        case IDLE:
            break; // Do nothing.

        case LED_OFF:
            ledsOff();
            break;

        case RAINBOW_CYCLE:
            ptrRainbow->run();
            break;

        case WHITE_OVER_RAINBOW:
            ptrWhiteRainbow->run();
            break;

        case COLOR:
            colorWipe(currentColor);
            break;

        case COLOR_WIPE:
            for (S32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                colorWipe(strips[n]->Color(255, 0, 0),     255);  // Red
                colorWipe(strips[n]->Color(0, 255, 0),     255);  // Green
                colorWipe(strips[n]->Color(0, 0, 255),     255);  // Blue
                colorWipe(strips[n]->Color(255, 255, 255), 255);  // White
            }
            break;

        case PULSE_WHITE:
            pulseWhite();
            break;

        case WHITE:
            fullWhite();
            break;

        default:
            currentMode = LED_OFF;
            break;
    }

    if ((RAINBOW_CYCLE != currentMode) &&
        (WHITE_OVER_RAINBOW != currentMode))
    {
        currentMode = IDLE;  // Each state runs once. TODO: Split states into separate state machines.
    }
}


LEDModes LEDControl::getMode(void)
{
    return currentMode;
}


void LEDControl::setLedMode(LEDModes newMode, U32 color)
{
    currentMode = newMode;

    if (COLOR == currentMode)
    {
        currentColor = color;
    }
}


void LEDControl::setBrightness(U32 brightness)
{
    currentBrightness = (brightness > MAX_LED_BRIGHTNESS) ? MAX_LED_BRIGHTNESS : brightness;

    for (S32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n]->setBrightness(currentBrightness);
        strips[n]->show();
    }
}


void LEDControl::setWhiteRainbowLength(U32 length)
{
    if (NULL != ptrWhiteRainbow)
    {
        reinterpret_cast<SMWhiteOverRainbow*>(ptrWhiteRainbow)->setWhiteLength(length);
    }
}


void LEDControl::setWhiteRainbowSpeed(Msec speed)
{
    if (NULL != ptrWhiteRainbow)
    {
        reinterpret_cast<SMWhiteOverRainbow*>(ptrWhiteRainbow)->setWhiteSpeed(speed);
    }
}


U8 LEDControl::getRed(U32 color)
{
    return (color >> 16);
}


U8 LEDControl::getGreen(U32 color)
{
    return (color >> 8);
}


U8 LEDControl::getBlue(U32 color)
{
    return (color);
}


// Convert separate R,G,B into packed 32-bit RGB color.
U32 LEDControl::getColor(U8 r, U8 g, U8 b)
{
    return ((U32)r << 16) | ((U32)g <<  8) | b;
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


void LEDControl::ledsOff(void)
{
    // Turn all pixels OFF
    // TODO: Investigate setting the brightness for each strip to 0.
    colorWipe(0);
}


// Fill the dots one after the other with a color
void LEDControl::colorWipe(U32 c, U8 wait)
{
    for (S32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        for (U16 j = 0; j < strips[n]->numPixels(); j++)
        {
            strips[n]->setPixelColor(j, c);
        }

        strips[n]->show();
    }
    
    delay(wait);
}


void LEDControl::pulseWhite(void)
{
    U32 color = 0x00000000;  // OFF

    for (S32 j = 0; j < 256 ; j++)
    {
        color = getColor(neopix_gamma[j], neopix_gamma[j], neopix_gamma[j]);

        for (S32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (U16 i = 0; i < strips[n]->numPixels(); i++)
            {
                strips[n]->setPixelColor(i, color);
            }
        }

        for (S32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n]->show();
        }

        delay(10);
    }

    for (S32 j = 255; j >= 0 ; j--)
    {
        color = getColor(neopix_gamma[j], neopix_gamma[j], neopix_gamma[j]);

        for (S32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (U16 i = 0; i < strips[n]->numPixels(); i++)
            {
                strips[n]->setPixelColor(i, color);
            }
        }

        for (S32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n]->show();
        }

        delay(10);
    }
}


void LEDControl::fullWhite(void)
{
    colorWipe(getColor(255, 255, 150));  // Warm white.
}


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
