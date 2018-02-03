//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "LEDControl.hpp"
#include "Adafruit_NeoPixel.h"
#include "appconfig.hpp"

#include "MaintenanceComm.hpp"  // TODO: Delete after debugging.
#include "stdio.h"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

#define NUM_LED_STRIPS      8
#define NUM_LEDS_PER_STRIP  29
#define MAX_LED_BRIGHTNESS  70  // Limit LED brightness to protect power supply.

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

Adafruit_NeoPixel* const strips[NUM_LED_STRIPS] =
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
    ledMode(IDLE),
    currBrightness(MAX_LED_BRIGHTNESS),
    currColor(0)  // Off
{
    // Do nothing.
}


void LEDControl::init(void)
{
    // Initialize all pixels to 'off'
    for (S32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n]->setBrightness(currBrightness);
        strips[n]->begin();
        strips[n]->show();
    }
}


void LEDControl::exec(void)
{
    // TODO: Break each state up to minimize time in this task. Only update one strip per frame?
    switch (ledMode)
    {
        case IDLE:
            break; // Do nothing.

        case LED_OFF:
            ledsOff();
            break;

        case RAINBOW_CYCLE:
            rainbowCycle(2);
            break;

        case WHITE_OVER_RAINBOW:
            whiteOverRainbow(75, 5);
            break;

        case COLOR:
            colorWipe(currColor);
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
            ledMode = LED_OFF;
            break;
    }

    ledMode = IDLE;  // Each state runs once. TODO: Split states into separate state machines.
}


void LEDControl::setLedMode(LEDModes newMode, U32 color)
{
    ledMode = newMode;

    if (COLOR == ledMode)
    {
        currColor = color;
    }
}


void LEDControl::setBrightness(U32 brightness)
{
    currBrightness = (brightness > MAX_LED_BRIGHTNESS) ? MAX_LED_BRIGHTNESS : brightness;

    for (S32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n]->setBrightness(currBrightness);
        strips[n]->show();
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


void LEDControl::whiteOverRainbow(Msec whiteSpeed, U8 whiteLength )
{
    static const U32 LOOPS = 3;
    static Msec lastTime = 0;

    U8 head;
    U8 tail = 0;
    U8 loopNum = 0;

    if (whiteLength >= NUM_LEDS_PER_STRIP)
    {
        whiteLength = NUM_LEDS_PER_STRIP - 1;
    }

    while (true)
    {
        for (U16 j = 0; j < 256; j++)
        {
            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (U16 i = 0; i < strips[n]->numPixels(); i++)
                {
                    if ((i >= tail && i <= head)   ||
                        (tail > head && i >= tail) ||
                        (tail > head && i <= head))
                    {
                        strips[n]->setPixelColor(i, getColor(255, 255, 255));
                    }
                    else
                    {
                        strips[n]->setPixelColor(i, Wheel(((i * 256 / strips[n]->numPixels()) + j) & 255, n));
                    }
                }

                if ((millis() - lastTime) > whiteSpeed)
                {
                    head++;
                    tail++;

                    if (head == strips[n]->numPixels())
                    {
                        loopNum++;
                    }

                    lastTime = millis();
                }

                if (LOOPS == loopNum)
                {
                    return;
                }
            }

            head %= NUM_LEDS_PER_STRIP;
            tail %= NUM_LEDS_PER_STRIP;

            for (S32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n]->show();
            }
        }
    }
}


void LEDControl::fullWhite(void)
{
    colorWipe(getColor(255, 255, 150));  // Warm white.
}


// Slightly different, this makes the rainbow equally distributed throughout
void LEDControl::rainbowCycle(U8 wait)
{
    for (U16 j = 0; j < 256 * 5; j++)  // 5 cycles of all colors on wheel
    {
        for (S32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (U16 i = 0; i < strips[n]->numPixels(); i++)
            {
                strips[n]->setPixelColor(i, Wheel(((i * 256 / strips[n]->numPixels()) + j) & 255, n));
            }
        }

        for (S32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n]->show();
            delay(wait);
        }
    }
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


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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
