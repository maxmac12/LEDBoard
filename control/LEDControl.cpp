//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "LEDControl.hpp"
#include "Adafruit_NeoPixel.h"
#include "appconfig.hpp"
#include "MaintenanceComm.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------
#define MOM_SW_PIN       A0  // Momentary switch pin.
#define MOM_SW_DEBOUNCE  40  // Debounce time (Msec) for momentary switch.

#define BRIGHTNESS_PIN        A1  // Brightness potentiometer pin.
#define BRIGHTNESS_TOLERANCE  2   // +/- tolerance of change for brightness.
#define BRIGHTNESS_FLOOR      2   // Set brightness to minimum when less than this value.

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
    mode(OFF),
    whiteRainbowLength(DEFAULT_WHITE_RAINBOW_LENGTH),
    whiteRainbowSpeed(DEFAULT_WHITE_RAINBOW_SPEED),
    colorPulseSpeed(DEFAULT_COLOR_PULSE_SPEED),
    currentBrightness(MAX_LED_BRIGHTNESS),
    currentColor(0x00000000)
{
    // Do nothing.
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
}


LEDModes LEDControl::getLedMode(void)
{
    return mode;
}


void LEDControl::setLedMode(LEDModes newMode)
{
    mode = newMode;

    // Relay a message to the comm port to indicate LED mode change.
    switch (mode)
    {
        case OFF:
            maintComm->sendData("LED Mode -> OFF\r\n");
            break;

        case WHITE:
            maintComm->sendData("LED Mode -> White\r\n");
            break;

        case COLOR:
            S8 buf[MaintenanceComm::MAX_CONSOLE_LINE_LEN];  // Buffer for formatting strings.
            U8 red, green, blue;
            getColorChannels(currentColor, red, green, blue);
            snprintf(buf, MaintenanceComm::MAX_CONSOLE_LINE_LEN, "LED Mode -> Color (r: %d, g: %d, b: %d)\r\n", red, green, blue);
            maintComm->sendData(buf);
            break;

        case COLOR_PULSE:
            maintComm->sendData("LED Mode -> Pulse Color\r\n");
            break;

        case RAINBOW_CYCLE:
            maintComm->sendData("LED Mode -> Rainbow Cycle\r\n");
            break;

        case WHITE_OVER_RAINBOW:
            maintComm->sendData("LED Mode -> White Over Rainbow\r\n");
            break;

        default:
            break;  // Do nothing.
    }
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


U8 LEDControl::getBrightness(void)
{
    return currentBrightness;
}


void LEDControl::setWhiteRainbowLength(U32 length)
{
    // TODO: Need to reset state machine for this to take effect.
    whiteRainbowLength = (length >= NUM_LEDS_PER_STRIP) ? NUM_LEDS_PER_STRIP - 1 : length;
}


U32 LEDControl::getWhiteRainbowLength(void)
{
    return whiteRainbowLength;
}


void LEDControl::setWhiteRainbowSpeed(Msec speed)
{
    // TODO: Need to reset state machine for this to take effect.
    whiteRainbowSpeed = speed;
}


Msec LEDControl::getWhiteRainbowSpeed(void)
{
    return whiteRainbowSpeed;
}


void LEDControl::setPulseSpeed(Msec speed)
{
    // TODO: Need to reset state machine for this to take effect.
    colorPulseSpeed = speed;
}


Msec LEDControl::getPulseSpeed(void)
{
    return colorPulseSpeed;
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


void LEDControl::setCurrentColor(U32 c)
{
    currentColor = c;
}


void LEDControl::setCurrentColor(U8 r, U8 g, U8 b)
{
    currentColor = getColorValue(r, g, b);
}


void LEDControl::setStripHeight(U8 stripId, U8 height)
{
    if (stripId < NUM_LED_STRIPS)
    {
        stripHeight[stripId] = height;
    }
}


U8 LEDControl::getStripHeight(U8 stripId)
{
    return (stripId < NUM_LED_STRIPS) ? stripHeight[stripId] : 0;
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
