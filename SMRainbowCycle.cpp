//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SMRainbowCycle.hpp"
#include "LEDControl.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

//############################################################################
//  Public Methods
//############################################################################

SMRainbowCycle::SMRainbowCycle():
    ptrCurrentState(NULL),
    currentStrip(0),
    wheelPosition(0)
{
    // Initialize the pointers to the rainbow cycle state functions.
    ptrStateFunc[IDLE_STATE]    = &SMRainbowCycle::idle;
    ptrStateFunc[INITIAL_STATE] = &SMRainbowCycle::initialState;
    ptrStateFunc[UPDATE_PIXELS] = &SMRainbowCycle::updatePixels;
    ptrStateFunc[UPDATE_STRIPS] = &SMRainbowCycle::updateStrips;

    // Set the initial rainbow cycle state.
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMRainbowCycle::init(void)
{
    // Do nothing.
}


void SMRainbowCycle::run(void)
{
    if (NULL != ptrCurrentState)
    {
        // Execute the current state.
        (this->*ptrCurrentState)();
    }
}


void SMRainbowCycle::reset(void)
{
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void SMRainbowCycle::idle(void)
{
    if (RAINBOW_CYCLE == ledCtrl->getLedMode())
    {
        ptrCurrentState = ptrStateFunc[INITIAL_STATE];
    }
}


void SMRainbowCycle::initialState(void)
{
    // Begin updating the pixels starting with the first pixel on the first LED strip.
    currentStrip  = 0;
    wheelPosition = 0;

    ptrCurrentState = ptrStateFunc[UPDATE_PIXELS];
}


void SMRainbowCycle::updatePixels(void)
{
    // Update one strip per cycle.
    U32 color;
    U32 numPixels = ledCtrl->getNumPixels(currentStrip);

    for (U16 i = 0; i < numPixels; i++)
    {
        color = ledCtrl->Wheel(((i * 256 / numPixels) + wheelPosition) & 255, currentStrip);

        ledCtrl->setPixelColor(currentStrip, i, color);
    }

    ledCtrl->setCurrentColor(color);

    if (++currentStrip >= NUM_LED_STRIPS)
    {
        currentStrip = 0;
        ptrCurrentState = ptrStateFunc[UPDATE_STRIPS];
    }
}


void SMRainbowCycle::updateStrips(void)
{
    // Show all updated strips.
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
