//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SMOff.hpp"
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

SMOff::SMOff():
    ptrCurrentState(NULL)
{
    // Initialize the pointers to color state functions.
    ptrStateFunc[IDLE_STATE] = &SMOff::idle;
    ptrStateFunc[OFF_STATE]  = &SMOff::turnLedsOff;

    // Set the initial color state.
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMOff::init(void)
{
    // Do nothing.
}


void SMOff::run(void)
{
    if (NULL != ptrCurrentState)
    {
        // Execute the current state.
        (this->*ptrCurrentState)();
    }
}


void SMOff::reset(void)
{
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}

//############################################################################
//  Protected Methods
//############################################################################

void SMOff::idle(void)
{
    if (OFF == ledCtrl->getLedMode())
    {
        ptrCurrentState = ptrStateFunc[OFF_STATE];
    }
}

//############################################################################
//  Private Methods
//############################################################################


void SMOff::turnLedsOff(void)
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

    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}
