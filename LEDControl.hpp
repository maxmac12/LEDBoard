#ifndef LEDCONTROL_H
#define LEDCONTROL_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "Adafruit_NeoPixel.h"
#include "Singleton.hpp"
#include "dataTypes.h"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

#define NUM_LED_STRIPS      8
#define NUM_LEDS_PER_STRIP  29
#define MAX_LED_BRIGHTNESS  100  // [1 (min)- 255 (max] Limit LED brightness to protect power supply.
#define MIN_LED_BRIGHTNESS  1    // TODO: Determine if Adafruit Neopixel library has issues with brightness of 0.

#define DEFAULT_WHITE_RAINBOW_SPEED   20
#define DEFAULT_WHITE_RAINBOW_LENGTH  4

#define DEFAULT_COLOR_PULSE_SPEED  20  // Pulse Time = 2 * pulseSpeed * MAX_LED_BRIGHTNESS [msecs]

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

// The following enumerations indicate the different LED modes.
// This list must begin with zero (0) and be in sequential order
// in order for the momentary switch to properly cycle through modes.
enum LEDModes
{
    OFF = 0,
    WHITE,
    COLOR,
    COLOR_PULSE,
    RAINBOW_CYCLE,
    WHITE_OVER_RAINBOW,

    NUM_LED_MODES
};

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class LEDControl
{
    public:

        explicit LEDControl();

        void init(void);
        LEDModes getLedMode(void);
        void setLedMode(LEDModes mode);
        void setBrightness(U8 brightness);
        U8 getBrightness(void);
        void setWhiteRainbowLength(U32 length);
        U32 getWhiteRainbowLength(void);
        void setWhiteRainbowSpeed(Msec speed);
        Msec getWhiteRainbowSpeed(void);
        void setPulseSpeed(Msec speed);
        Msec getPulseSpeed(void);
        U8  getRedChannel(U32 color);
        U8  getGreenChannel(U32 color);
        U8  getBlueChannel(U32 color);
        U32 getColorValue(U8 r, U8 g, U8 b);
        void getColorChannels(U32 color, U8& r, U8& g, U8& b);
        U32 getCurrentColor(void);
        void setCurrentColor(U8 c);
        void setCurrentColor(U8 r, U8 g, U8 b);
        U32 getNumPixels(U32 stripId);
        void setPixelColor(U32 stripId, U32 pixelId, U32 color);
        void updateStrip(U32 stripId);
        void updateAllStrips(void);
        U32 Wheel(S8 WheelPos, U32 stripId);
        void turnLedsOff(void);

        // Unused and disabled.
        ~LEDControl() {}
        LEDControl(const LEDControl&);
        LEDControl& operator=(const LEDControl&);

    protected:

    private:

        void readAnalogBrightness(void);
        void readMomentarySwitch(void);
        void rainbow(U8 wait);

        LEDModes mode;
        U32 whiteRainbowLength;
        Msec whiteRainbowSpeed;
        Msec colorPulseSpeed;
        U8 currentBrightness;
        U32 currentColor;
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#define ledCtrl Singleton<LEDControl>::getInstance()

#endif
