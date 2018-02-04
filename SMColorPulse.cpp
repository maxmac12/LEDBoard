//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SMColorPulse.hpp"
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

SMColorPulse::SMColorPulse():
    ptrCurrentState(NULL),
    brightness(1),
    pulseSpeed(50),  // Pulse Time = 2 * pulseSpeed * MAX_LED_BRIGHTNESS [msecs]
    fadeInFlag(true)
{
    // Initialize the pointers to the color pulse state functions.
    ptrStateFunc[IDLE_STATE]     = &SMColorPulse::idle;
    ptrStateFunc[INITIAL_STATE]  = &SMColorPulse::initialState;
    ptrStateFunc[FADE_IN_STATE]  = &SMColorPulse::fadeIn;
    ptrStateFunc[FADE_OUT_STATE] = &SMColorPulse::fadeOut;
    ptrStateFunc[DELAY_STATE]    = &SMColorPulse::delay;

    // Set the initial color pulse state.
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMColorPulse::init(void)
{
    // Do nothing.
}


void SMColorPulse::run(void)
{
    if (NULL != ptrCurrentState)
    {
        // Execute the current state.
        (this->*ptrCurrentState)();
    }
}


void SMColorPulse::reset(void)
{
    ptrCurrentState = ptrStateFunc[IDLE_STATE];
}


void SMColorPulse::setPulseSpeed(Msec speed)
{
    pulseSpeed = speed;
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void SMColorPulse::idle(void)
{
    if (COLOR_PULSE == ledCtrl->getLedMode())
    {
        ptrCurrentState = ptrStateFunc[INITIAL_STATE];
    }
}


void SMColorPulse::initialState(void)
{
    brightness = 1;
    fadeInFlag = true;
    stopWatch.start(pulseSpeed);

    ptrCurrentState = ptrStateFunc[FADE_IN_STATE];
}


void SMColorPulse::fadeIn(void)
{
    if (++brightness >= MAX_LED_BRIGHTNESS)
    {
        fadeInFlag = false;
    }

    ptrCurrentState = ptrStateFunc[DELAY_STATE];
}


void SMColorPulse::fadeOut(void)
{
    if (--brightness <= MIN_LED_BRIGHTNESS)
    {
        fadeInFlag = true;;
    }

    ptrCurrentState = ptrStateFunc[DELAY_STATE];
}


void SMColorPulse::delay(void)
{
    if (stopWatch.timerHasExpired())
    {
        ledCtrl->setBrightness(brightness);
        ledCtrl->updateAllStrips();
        stopWatch.start(pulseSpeed);

        if (fadeInFlag)
        {
            ptrCurrentState = ptrStateFunc[FADE_IN_STATE];
        }
        else
        {
            ptrCurrentState = ptrStateFunc[FADE_OUT_STATE];
        }
    }
}
