#ifndef TESTADAFRUIT_H
#define TESTADAFRUIT_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "Adafruit_NeoPixel.h"
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

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class Test_Adafruit
{
    public:

        Test_Adafruit();

        void init(void);
        void exec(void);

        // Unused and disabled.
        ~Test_Adafruit() {}
        Test_Adafruit(const Test_Adafruit&);
        Test_Adafruit& operator=(const Test_Adafruit&);

    protected:

    private:

        Adafruit_NeoPixel* strips[NUM_LED_STRIPS];

        U8 red(U32 c);
        U8 green(U32 c);
        U8 blue(U32 c);
        U32 Wheel(S8 WheelPos, int stripId);
        void rainbow(U8 wait);
        void rainbowCycle(U8 wait);
        void fullWhite(void);
        void whiteOverRainbow(U8 wait, U8 whiteSpeed, U8 whiteLength );
        void rainbowFade2White(U8 wait, int rainbowLoops, int whiteLoops);
        void pulseWhite(U8 wait);
        void colorWipe(U32 c, U8 wait);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
