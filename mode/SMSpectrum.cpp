//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SMSpectrum.hpp"
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

SMSpectrum::SMSpectrum():
    ptrCurrentState(NULL),
    currentStrip(0)
{
    // Initialize the pointers to the spectrum state functions.
    ptrStateFunc[IDLE_STATE]    = &SMSpectrum::idle;
    ptrStateFunc[INITIAL_STATE] = &SMSpectrum::initialState;
    ptrStateFunc[UPDATE_PIXELS] = &SMSpectrum::updatePixels;
    ptrStateFunc[UPDATE_STRIPS] = &SMSpectrum::updateStrips;

    // Set the initial color state.
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMSpectrum::init(void)
{
    // Do nothing.
}


void SMSpectrum::run(void)
{
    if (NULL != ptrCurrentState)
    {
        // Execute the current state.
        (this->*ptrCurrentState)();
    }
}


void SMSpectrum::reset(void)
{
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void SMSpectrum::idle(void)
{
    if (SPECTRUM == ledCtrl->getLedMode())
    {
        ptrCurrentState = ptrStateFunc[INITIAL_STATE];
    }
}

void SMSpectrum::initialState(void)
{
    // Begin updating the pixels starting with the first pixel on the first LED strip.
    currentStrip  = 0;

    ptrCurrentState = ptrStateFunc[UPDATE_PIXELS];
}


void SMSpectrum::updatePixels(void)
{
    // Update one strip per cycle.
    U32 color;
    U32 numPixels = ledCtrl->getNumPixels(currentStrip);

    for (U16 i = 0; i < numPixels; i++)
    {
        if (i < ledCtrl->getStripHeight(currentStrip))
        {
            // LED should be colored.
            color = ledCtrl->getCurrentColor();
        }
        else
        {
            color = 0;
        }

        ledCtrl->setPixelColor(currentStrip, i, color);
    }

    if (++currentStrip >= NUM_LED_STRIPS)
    {
        currentStrip = 0;
        ptrCurrentState = ptrStateFunc[UPDATE_STRIPS];
    }
}


void SMSpectrum::updateStrips(void)
{
    // Show all updated strips.
    for (S32 n = 0; n < NUM_LED_STRIPS; n++)
    {
        ledCtrl->updateStrip(n);
    }

    ptrCurrentState = ptrStateFunc[UPDATE_PIXELS];
}
