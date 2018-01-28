/*
 * RGBWStripTest.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: Max
 */

#include "Adafruit_NeoPixel.h"
#include <HardwareSerial.h>

#define STRIP_1_PIN  2
#define STRIP_2_PIN  14
#define STRIP_3_PIN  7
#define STRIP_4_PIN  8
#define STRIP_5_PIN  6
#define STRIP_6_PIN  20
#define STRIP_7_PIN  21
#define STRIP_8_PIN  5

#define NUM_LED_STRIPS 8

#define NUM_LEDS_PER_STRIP 29

#define BRIGHTNESS 50

Adafruit_NeoPixel strips[NUM_LED_STRIPS] = { Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_1_PIN, NEO_GRB + NEO_KHZ800),
                                             Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_2_PIN, NEO_GRB + NEO_KHZ800),
                                             Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_3_PIN, NEO_GRB + NEO_KHZ800),
                                             Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_4_PIN, NEO_GRB + NEO_KHZ800),
                                             Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_5_PIN, NEO_GRB + NEO_KHZ800),
                                             Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_6_PIN, NEO_GRB + NEO_KHZ800),
                                             Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_7_PIN, NEO_GRB + NEO_KHZ800),
                                             Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, STRIP_8_PIN, NEO_GRB + NEO_KHZ800) };

byte neopix_gamma[] = {
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

static uint8_t red(uint32_t c);
static uint8_t green(uint32_t c);
static uint8_t blue(uint32_t c);
static uint32_t Wheel(byte WheelPos, int stripId);
static void rainbow(uint8_t wait);
static void rainbowCycle(uint8_t wait);
static void fullWhite(void);
static void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength );
static void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops);
static void pulseWhite(uint8_t wait);
static void colorWipe(uint32_t c, uint8_t wait);


void setup(void)
{
    // Setup serial ports.
    Serial1.begin(115200);  // RX1/TX1 (USB)
    Serial2.begin(115200);  // RX2/TX2 (TTL)
    Serial1.printf("USB Initialized\r\n");
    Serial2.printf("XBEE Initialized\r\n");

    for (int n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n].setBrightness(BRIGHTNESS);
        strips[n].begin();
        strips[n].show(); // Initialize all pixels to 'off'
    }
}


void loop(void)
{
    // Some example procedures showing how to display to the pixels:
    for (int n = 0; n < NUM_LED_STRIPS; n++)
    {
        colorWipe(strips[n].Color(255, 0, 0), 255);    // Red
        colorWipe(strips[n].Color(0, 255, 0), 255);    // Green
        colorWipe(strips[n].Color(0, 0, 255), 255);    // Blue
        colorWipe(strips[n].Color(0, 0, 0, 255), 255); // White
    }
//
    whiteOverRainbow(20, 75, 5);
//
    pulseWhite(5);
//
//    //fullWhite();
//    //delay(2000);

    rainbowFade2White(3, 3, 1);
}


// Fill the dots one after the other with a color
static void colorWipe(uint32_t c, uint8_t wait)
{
    for (int n = 0; n < NUM_LED_STRIPS; n++)
    {
        for (uint16_t j = 0; j < strips[n].numPixels(); j++)
        {
            strips[n].setPixelColor(j, c);
        }

        strips[n].show();
    }

    delay(wait);
}


static void pulseWhite(uint8_t wait)
{
    for (int j = 0; j < 256 ; j++)
    {
        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (uint16_t i = 0; i < strips[n].numPixels(); i++)
            {
                strips[n].setPixelColor(i, strips[n].Color(0,0,0, neopix_gamma[j]));
            }
        }

        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n].show();
        }

        delay(wait);
    }

    for (int j = 255; j >= 0 ; j--)
    {
        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (uint16_t i = 0; i < strips[n].numPixels(); i++)
            {
                strips[n].setPixelColor(i, strips[n].Color(0,0,0, neopix_gamma[j]));
            }
        }

        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n].show();
        }

        delay(wait);
    }
}


static void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops)
{
    float fadeMax = 100.0;
    int fadeVal = 0;
    int redVal;
    int greenVal;
    int blueVal;
    uint32_t wheelVal;

    for (int k = 0 ; k < rainbowLoops ; k ++)
    {
        for (int j = 0; j < 256; j++) // 5 cycles of all colors on wheel
        {
            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (uint16_t i = 0; i < strips[n].numPixels(); i++)
                {
                    wheelVal = Wheel(((i * 256 / strips[n].numPixels()) + j) & 255, n);

                    redVal   = red(wheelVal) * float(fadeVal/fadeMax);
                    greenVal = green(wheelVal) * float(fadeVal/fadeMax);
                    blueVal  = blue(wheelVal) * float(fadeVal/fadeMax);

                    strips[n].setPixelColor(i, strips[n].Color(redVal, greenVal, blueVal));
                }
            }

            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n].show();
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

    for (int k = 0 ; k < whiteLoops ; k ++)
    {
        for (int j = 0; j < 256 ; j++)
        {
            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (uint16_t i = 0; i < strips[n].numPixels(); i++)
                {
                    strips[n].setPixelColor(i, strips[n].Color(0, 0, 0, neopix_gamma[j]));
                }
            }

            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n].show();
            }
        }

        delay(2000);

        for (int j = 255; j >= 0 ; j--)
        {
            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                for (uint16_t i = 0; i < strips[n].numPixels(); i++)
                {
                    strips[n].setPixelColor(i, strips[n].Color(0, 0, 0, neopix_gamma[j]));
                }
            }

            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n].show();
            }
        }
    }

    delay(500);
}


static void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength )
{
    static const int LOOPS = 3;
    static unsigned long lastTime = 0;

    int head;
    int tail = 0;
    int loopNum = 0;

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
                for (uint16_t i = 0; i < strips[n].numPixels(); i++)
                {
                    if ((i >= tail && i <= head)   ||
                        (tail > head && i >= tail) ||
                        (tail > head && i <= head))
                    {
                        strips[n].setPixelColor(i, strips[n].Color(0,0,0, 255));
                    }
                    else
                    {
                        strips[n].setPixelColor(i, Wheel(((i * 256 / strips[n].numPixels()) + j) & 255, n));
                    }
                }

                if ((millis() - lastTime) > whiteSpeed)
                {
                    head++;
                    tail++;

                    if (head == strips[n].numPixels())
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

            for (int n = 0; n < NUM_LED_STRIPS; n++)
            {
                strips[n].show();
                delay(wait);
            }
        }
    }
}


static void fullWhite(void)
{
    for (int n = 0; n < NUM_LED_STRIPS; n++)
    {
        for (uint16_t i = 0; i < strips[n].numPixels(); i++)
        {
            strips[n].setPixelColor(i, strips[n].Color(0,0,0, 255));
        }
    }

    for (int n = 0; n < NUM_LED_STRIPS; n++)
    {
        strips[n].show();
    }
}


// Slightly different, this makes the rainbow equally distributed throughout
static void rainbowCycle(uint8_t wait)
{
    for (uint16_t j = 0; j < 256 * 5; j++)  // 5 cycles of all colors on wheel
    {
        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (uint16_t i = 0; i < strips[n].numPixels(); i++)
            {
                strips[n].setPixelColor(i, Wheel(((i * 256 / strips[n].numPixels()) + j) & 255, n));
            }
        }

        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n].show();
            delay(wait);
        }
    }
}


static void rainbow(uint8_t wait)
{
    for (uint16_t j = 0; j < 256; j++)
    {
        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            for (uint16_t i = 0; i < strips[n].numPixels(); i++)
            {
                strips[n].setPixelColor(i, Wheel((i+j) & 255, n));
            }
        }

        for (int n = 0; n < NUM_LED_STRIPS; n++)
        {
            strips[n].show();
            delay(wait);
        }
    }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
static uint32_t Wheel(byte WheelPos, int stripId)
{
    WheelPos = 255 - WheelPos;

    if (WheelPos < 85)
    {
        return strips[stripId].Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
    }

    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strips[stripId].Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
    }

    WheelPos -= 170;
    return strips[stripId].Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}


static uint8_t red(uint32_t c)
{
    return (c >> 16);
}


static uint8_t green(uint32_t c)
{
    return (c >> 8);
}


static uint8_t blue(uint32_t c)
{
    return (c);
}


