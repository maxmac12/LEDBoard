//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SMColor.hpp"
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

SMColor::SMColor():
    ptrCurrentState(NULL),
    currentColor(0x00000000),  // OFF
    runUntilColorChange(true)
{
    // Initialize the pointers to color state functions.
    ptrStateFunc[IDLE_STATE]      = &SMColor::idle;
    ptrStateFunc[SET_COLOR_STATE] = &SMColor::setColor;

    // Set the initial color state.
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMColor::init(void)
{
    // Set all pixels OFF.
    for (S32 i = 0; i < NUM_LED_STRIPS; i++)
    {
        U16 numPixels = ledCtrl->getNumPixels(i);

        for (U16 j = 0; j < numPixels; j++)
        {
            ledCtrl->setPixelColor(i, j, currentColor);
        }

        ledCtrl->updateStrip(i);
    }
}


void SMColor::run(void)
{
    if (NULL != ptrCurrentState)
    {
        // Execute the current state.
        (this->*ptrCurrentState)();
    }
}


void SMColor::reset(void)
{
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void SMColor::idle(void)
{
    if ((COLOR == ledCtrl->getLedMode()) &&
       ((currentColor != ledCtrl->getCurrentColor()) || runUntilColorChange))
    {
        // This block allows the state machine to run until a change in color occurs
        // and is needed
        if (currentColor != ledCtrl->getCurrentColor())
        {
            runUntilColorChange = false;
        }

        ptrCurrentState = ptrStateFunc[SET_COLOR_STATE];
    }
}


void SMColor::setColor(void)
{
    currentColor = ledCtrl->getCurrentColor();

    for (S32 i = 0; i < NUM_LED_STRIPS; i++)
    {
        U16 numPixels = ledCtrl->getNumPixels(i);

        for (U16 j = 0; j < numPixels; j++)
        {
            ledCtrl->setPixelColor(i, j, currentColor);
        }

        ledCtrl->updateStrip(i);
    }

    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}
