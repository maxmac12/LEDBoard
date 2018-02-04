#ifndef TESTADAFRUIT_H
#define TESTADAFRUIT_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "Adafruit_NeoPixel.h"
#include "Task.hpp"
#include "Singleton.hpp"
#include "StateMachine.hpp"
#include "StopWatch.hpp"
#include "dataTypes.h"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

#define NUM_LED_STRIPS      8
#define NUM_LEDS_PER_STRIP  29
#define MAX_LED_BRIGHTNESS  100  // [1 (min)- 255 (max] Limit LED brightness to protect power supply.
#define MIN_LED_BRIGHTNESS  1    // Adafruit Neopixel library has issues with brightness of 0.

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

enum LEDModes
{
    COLOR = 0,
    COLOR_PULSE,
    RAINBOW_CYCLE,
    WHITE_OVER_RAINBOW,

    NUM_LED_MODES
};

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class LEDControl : public Task
{
    public:

        explicit LEDControl();

        void init(void);
        void exec(void);
        LEDModes getLedMode(void);
        void setLedMode(LEDModes mode);
        void setBrightness(U8 brightness);
        void setWhiteRainbowLength(U32 length);
        void setWhiteRainbowSpeed(Msec speed);
        void setPulseSpeed(Msec speed);
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

        // Unused and disabled.
        ~LEDControl() {}
        LEDControl(const LEDControl&);
        LEDControl& operator=(const LEDControl&);

    protected:

    private:

        void readAnalogBrightness(void);
        void rainbow(U8 wait);

        StateMachine* ptrLedStateMachines[NUM_LED_MODES];
        LEDModes currentMode;
        LEDModes previousMode;
        U8 currentBrightness;
        U32 currentColor;
        StopWatch* ptrStopWatch;
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#define ledCtrl Singleton<LEDControl>::getInstance()

#endif
