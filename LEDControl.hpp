#ifndef TESTADAFRUIT_H
#define TESTADAFRUIT_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "Adafruit_NeoPixel.h"
#include "Task.hpp"
#include "Singleton.hpp"
#include "dataTypes.h"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

enum LEDModes
{
    IDLE,
    LED_OFF,
    RAINBOW_CYCLE,
    WHITE_OVER_RAINBOW,
    PULSE_WHITE,
    WHITE,
    COLOR,
    COLOR_WIPE
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
        void setLedMode(LEDModes mode, U32 color = 0);
        void setBrightness(U32 brightness);
        U8  getRed(U32 color);
        U8  getGreen(U32 color);
        U8  getBlue(U32 color);
        U32 getColor(U8 r, U8 g, U8 b);

        // Unused and disabled.
        ~LEDControl() {}
        LEDControl(const LEDControl&);
        LEDControl& operator=(const LEDControl&);

    protected:

    private:

        LEDModes ledMode;
        U32 currBrightness;
        U32 currColor;

        U32 Wheel(S8 WheelPos, U32 stripId);
        void rainbow(U8 wait);
        void rainbowCycle(U8 wait);
        void fullWhite(void);
        void whiteOverRainbow(Msec whiteSpeed, U8 whiteLength);
        void pulseWhite(void);
        void colorWipe(U32 c, U8 wait = 0);
        void ledsOff(void);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#define ledCtrl Singleton<LEDControl>::getInstance()

#endif
