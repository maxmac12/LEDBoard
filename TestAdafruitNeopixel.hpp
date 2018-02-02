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
    LED_OFF,
    RAINBOW_CYCLE,
    WHITE_OVER_RAINBOW,
    RAINBOW_TO_WHITE,
    COLOR_WIPE,
    PULSE_WHITE
};

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class Test_Adafruit : public Task
{
    public:

        explicit Test_Adafruit();

        void init(void);
        void exec(void);
        void setLedMode(LEDModes mode);

        // Unused and disabled.
        ~Test_Adafruit() {}
        Test_Adafruit(const Test_Adafruit&);
        Test_Adafruit& operator=(const Test_Adafruit&);

    protected:

    private:

        Adafruit_NeoPixel* strips[NUM_LED_STRIPS];
        LEDModes ledMode;

        U8 red(U32 c);
        U8 green(U32 c);
        U8 blue(U32 c);
        U32 Wheel(S8 WheelPos, U32 stripId);
        void rainbow(U8 wait);
        void rainbowCycle(U8 wait);
        void fullWhite(void);
        void whiteOverRainbow(U8 wait, Msec whiteSpeed, U8 whiteLength );
        void rainbowFade2White(U8 wait, U32 rainbowLoops, U32 whiteLoops);
        void pulseWhite(U8 wait);
        void colorWipe(U32 c, U8 wait);
        void ledsOff(void);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#define ledCtrl Singleton<Test_Adafruit>::getInstance()

#endif
