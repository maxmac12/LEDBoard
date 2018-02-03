//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SMWhiteOverRainbow.hpp"
#include "LEDControl.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------
#define DEFAULT_SPEED   50
#define DEFAULT_LENGTH  4

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

//############################################################################
//  Public Methods
//############################################################################

SMWhiteOverRainbow::SMWhiteOverRainbow():
    ptrCurrentState(NULL),
    currentStrip(0),
    wheelPosition(0),
    whiteSpeed(DEFAULT_SPEED),
    whiteLength(DEFAULT_LENGTH),
    head(0),
    tail(0)
{
    // Initialize the pointers to the rainbow cycle state functions.
    ptrStateFunc[IDLE_STATE]    = &SMWhiteOverRainbow::idle;
    ptrStateFunc[INITIAL_STATE] = &SMWhiteOverRainbow::initialState;
    ptrStateFunc[UPDATE_PIXELS] = &SMWhiteOverRainbow::updatePixels;
    ptrStateFunc[UPDATE_STRIPS] = &SMWhiteOverRainbow::updateStrips;

    // Set the initial rainbow cycle state.
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMWhiteOverRainbow::init(void)
{
    // Do nothing.
}


void SMWhiteOverRainbow::run(void)
{
    if (NULL != ptrCurrentState)
    {
        // Execute the current state.
        (this->*ptrCurrentState)();
    }
}


void SMWhiteOverRainbow::reset(void)
{
    ptrCurrentState = ptrStateFunc[IDLE];
}


void SMWhiteOverRainbow::setWhiteLength(U32 length)
{
    whiteLength = (length >= NUM_LEDS_PER_STRIP) ? NUM_LEDS_PER_STRIP - 1 : length;
    reset();
}


void SMWhiteOverRainbow::setWhiteSpeed(Msec speed)
{
    whiteSpeed = speed;
    reset();
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void SMWhiteOverRainbow::idle(void)
{
    if (WHITE_OVER_RAINBOW == ledCtrl->getMode())
    {
        ptrCurrentState = ptrStateFunc[INITIAL_STATE];
    }
}


void SMWhiteOverRainbow::initialState(void)
{
    // Begin updating the pixels starting with the first pixel on the first LED strip.
    currentStrip  = 0;
    wheelPosition = 0;
    head          = whiteLength - 1;
    tail          = 0;
    stopWatch.start(whiteSpeed);

    ptrCurrentState = ptrStateFunc[UPDATE_PIXELS];
}


void SMWhiteOverRainbow::updatePixels(void)
{
    // Update one strip per cycle.
    U32 numPixels = ledCtrl->getNumPixels(currentStrip);

    for (U16 i = 0; i < numPixels; i++)
    {
        if ((i >= tail && i <= head)   ||
            (tail > head && i >= tail) ||
            (tail > head && i <= head))
        {
            // Set pixel to WHITE.
            ledCtrl->setPixelColor(currentStrip,
                                   i,
                                   ledCtrl->getColor(255, 255, 255));
        }
        else
        {
            // Set pixel to the current color wheel color.
            ledCtrl->setPixelColor(currentStrip,
                                   i,
                                   ledCtrl->Wheel(((i * 256 / numPixels) + wheelPosition) & 255, currentStrip));
        }
    }

    if (stopWatch.timerHasExpired())
    {
        head++;  // Expected to rollover.
        tail++;  // Expected to rollover.
        stopWatch.start(whiteSpeed);
    }

    if (++currentStrip >= NUM_LED_STRIPS)
    {
        currentStrip = 0;
        ptrCurrentState = ptrStateFunc[UPDATE_STRIPS];
    }
}


void SMWhiteOverRainbow::updateStrips(void)
{
    head %= NUM_LEDS_PER_STRIP;
    tail %= NUM_LEDS_PER_STRIP;

    // Show all updated strips
    for (S32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        ledCtrl->updateStrip(n);
    }

    if (++wheelPosition >= 256)
    {
        wheelPosition = 0;
    }

    ptrCurrentState = ptrStateFunc[UPDATE_PIXELS];
}
