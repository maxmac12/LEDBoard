//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SMWhite.hpp"
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

SMWhite::SMWhite()
{
    // Initialize the pointers to color state functions.
    ptrStateFunc[IDLE_STATE]  = &SMWhite::idle;
    ptrStateFunc[WHITE_STATE] = &SMWhite::setWhiteLight;

    // Set the initial color state.
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMWhite::init(void)
{
    // Do nothing.
}


void SMWhite::run(void)
{
    if (NULL != ptrCurrentState)
    {
        // Execute the current state.
        (this->*ptrCurrentState)();
    }
}


void SMWhite::reset(void)
{
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void SMWhite::idle(void)
{
    if (WHITE == ledCtrl->getLedMode())
    {
        ptrCurrentState = ptrStateFunc[WHITE_STATE];
    }
}


void SMWhite::setWhiteLight(void)
{
    for (S32 i = 0; i < NUM_LED_STRIPS; i++)
    {
        U16 numPixels = ledCtrl->getNumPixels(i);

        for (U16 j = 0; j < numPixels; j++)
        {
            // Set a warm white light color.
            ledCtrl->setPixelColor(i, j, ledCtrl->getColorValue(255, 255, 125));
        }

        ledCtrl->updateStrip(i);
    }

    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}
