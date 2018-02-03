//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "LEDControl.hpp"

#include "Adafruit_NeoPixel.h"
#include "appconfig.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

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

const U8 ledStripPins[NUM_LED_STRIPS] = { 2,     // LED Strip 1 pin
                                          14,    // LED Strip 2 pin
                                          7,     // LED Strip 3 pin
                                          8,     // LED Strip 4 pin
                                          6,     // LED Strip 5 pin
                                          20,    // LED Strip 6 pin
                                          21,    // LED Strip 7 pin
                                          5  };  // LED Strip 8 pin

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
    ledMode(LED_OFF)
{
    for (U32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n] = new Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, ledStripPins[n], NEO_GRB + NEO_KHZ800);
    }
}


void LEDControl::init(void)
{
    // Initialize all pixels to 'off'
    for (U32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n]->setBrightness(LED_BRIGHTNESS);
        strips[n]->begin();
        strips[n]->show();
    }
}


void LEDControl::exec(void)
{
    // TODO: Break each state up to minimize time in this task. Only update one strip per frame?
    switch (ledMode)
    {
        case LED_OFF:
            ledsOff();
            break;

        case RAINBOW_CYCLE:
            rainbowCycle(2);
            break;

        case WHITE_OVER_RAINBOW:
            whiteOverRainbow(20, 75, 5);
            break;

        case RAINBOW_TO_WHITE:
            rainbowFade2White(3, 3, 1);
            break;

        case COLOR_WIPE:
            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                colorWipe(strips[n]->Color(255, 0, 0), 255);    // Red
                colorWipe(strips[n]->Color(0, 255, 0), 255);    // Green
                colorWipe(strips[n]->Color(0, 0, 255), 255);    // Blue
                colorWipe(strips[n]->Color(0, 0, 0, 255), 255); // White
            }
            break;

        case PULSE_WHITE:
            pulseWhite(5);
            break;

        default:
            ledMode = LED_OFF;
            break;
    }
}


void LEDControl::setLedMode(LEDModes newMode)
{
    ledMode = newMode;
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

// Fill the dots one after the other with a color
void LEDControl::colorWipe(U32 c, U8 wait)
{
    for (U32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        for (U16 j = 0; j < strips[n]->numPixels(); j++)
        {
            strips[n]->setPixelColor(j, c);
        }

        strips[n]->show();
    }

    delay(wait);
}


void LEDControl::ledsOff(void)
{
    // Initialize all pixels to 'off'
    for (U32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        colorWipe(strips[n]->Color(0, 0, 0), 0);
        strips[n]->show();
    }
}


void LEDControl::pulseWhite(uint8_t wait)
{
    for (U32 j = 0; j < 256 ; j++)
    {
        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (U16 i = 0; i < strips[n]->numPixels(); i++)
            {
                strips[n]->setPixelColor(i, strips[n]->Color(0,0,0, neopix_gamma[j]));
            }
        }

        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n]->show();
        }

        delay(wait);
    }

    for (U32 j = 255; j >= 0 ; j--)
    {
        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (U16 i = 0; i < strips[n]->numPixels(); i++)
            {
                strips[n]->setPixelColor(i, strips[n]->Color(0,0,0, neopix_gamma[j]));
            }
        }

        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n]->show();
        }

        delay(wait);
    }
}


void LEDControl::rainbowFade2White(U8 wait, U32 rainbowLoops, U32 whiteLoops)
{
    float fadeMax = 100.0;
    U32 fadeVal = 0;
    U32 redVal;
    U32 greenVal;
    U32 blueVal;
    U32 wheelVal;

    for (U32 k = 0 ; k < rainbowLoops ; k ++)
    {
        for (U32 j = 0; j < 256; j++) // 5 cycles of all colors on wheel
        {
            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (U16 i = 0; i < strips[n]->numPixels(); i++)
                {
                    wheelVal = Wheel(((i * 256 / strips[n]->numPixels()) + j) & 255, n);

                    redVal   = red(wheelVal) * float(fadeVal/fadeMax);
                    greenVal = green(wheelVal) * float(fadeVal/fadeMax);
                    blueVal  = blue(wheelVal) * float(fadeVal/fadeMax);

                    strips[n]->setPixelColor(i, strips[n]->Color(redVal, greenVal, blueVal));
                }
            }

            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n]->show();
            }

            //First loop, fade in!
            if ((k == 0) &&
                (fadeVal < fadeMax - 1))
            {
                fadeVal++;
            }
            //Last loop, fade out!
            else if ((k == rainbowLoops - 1) &&
                     (j > 255 - fadeMax))
            {
                fadeVal--;
            }

            delay(wait);
        }
    }

    delay(500);

    for (U32 k = 0 ; k < whiteLoops ; k ++)
    {
        for (U32 j = 0; j < 256 ; j++)
        {
            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (U16 i = 0; i < strips[n]->numPixels(); i++)
                {
                    strips[n]->setPixelColor(i, strips[n]->Color(0, 0, 0, neopix_gamma[j]));
                }
            }

            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n]->show();
            }
        }

        delay(500);

        for (U32 j = 255; j >= 0 ; j--)
        {
            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (U16 i = 0; i < strips[n]->numPixels(); i++)
                {
                    strips[n]->setPixelColor(i, strips[n]->Color(0, 0, 0, neopix_gamma[j]));
                }
            }

            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n]->show();
            }
        }
    }

    delay(500);
}


void LEDControl::whiteOverRainbow(U8 wait, Msec whiteSpeed, U8 whiteLength )
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
        for (int j = 0; j < 256; j++)
        {
            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (uint16_t i = 0; i < strips[n]->numPixels(); i++)
                {
                    if ((i >= tail && i <= head)   ||
                        (tail > head && i >= tail) ||
                        (tail > head && i <= head))
                    {
                        strips[n]->setPixelColor(i, strips[n]->Color(0,0,0, 255));
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

            for (U32 n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n]->show();
                delay(wait);
            }
        }
    }
}


void LEDControl::fullWhite(void)
{
    for (U32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        for (U16 i = 0; i < strips[n]->numPixels(); i++)
        {
            strips[n]->setPixelColor(i, strips[n]->Color(0,0,0, 255));
        }
    }

    for (U32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n]->show();
    }
}


// Slightly different, this makes the rainbow equally distributed throughout
void LEDControl::rainbowCycle(U8 wait)
{
    for (U16 j = 0; j < 256 * 5; j++)  // 5 cycles of all colors on wheel
    {
        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (U16 i = 0; i < strips[n]->numPixels(); i++)
            {
                strips[n]->setPixelColor(i, Wheel(((i * 256 / strips[n]->numPixels()) + j) & 255, n));
            }
        }

        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
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
        for (U32 n = 0; n < NUM_LED_STRIPS; n++)
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


U8 LEDControl::red(U32 c)
{
    return (c >> 16);
}


U8 LEDControl::green(U32 c)
{
    return (c >> 8);
}


U8 LEDControl::blue(U32 c)
{
    return (c);
}


