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

#define STRIP_1_PIN  2
#define STRIP_2_PIN  14
#define STRIP_3_PIN  7
#define STRIP_4_PIN  8
#define STRIP_5_PIN  6
#define STRIP_6_PIN  20
#define STRIP_7_PIN  21
#define STRIP_8_PIN  5

#define NUM_LED_STRIPS  8
#define NUM_LEDS_PER_STRIP  29
#define LED_BRIGHTNESS  50

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
        void setLedMode(LEDModes mode);

        // Unused and disabled.
        ~LEDControl() {}
        LEDControl(const LEDControl&);
        LEDControl& operator=(const LEDControl&);

    protected:

    private:

        LEDModes ledMode;

        U8 getRed(U32 c);
        U8 getGreen(U32 c);
        U8 getBlue(U32 c);
        U32 Wheel(S8 WheelPos, U32 stripId);
        void rainbow(U8 wait);
        void rainbowCycle(U8 wait);
        void fullWhite(void);
        void whiteOverRainbow(Msec whiteSpeed, U8 whiteLength);
        void pulseWhite(void);
        void colorWipe(U32 c, U8 wait = 0);
        void ledsOff(void);
        U32 getColor(U8 r, U8 g, U8 b);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#define ledCtrl Singleton<LEDControl>::getInstance()

#endif
