//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "LedModeCommand.hpp"
#include "TestAdafruitNeopixel.hpp"

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

LedModeCommand::LedModeCommand()
{
    // do nothing
}


ErrorCode LedModeCommand::exec(const CStr subject, const CStr value1, const CStr value2)
{
    ErrorCode rtnErr = ER_SUCCESS;

    if (subject != NULL)
    {
        if (strncmp(subject, "off", 4UL) == 0)    // JSF151.1 Exception
        {
            ledCtrl->setLedMode(LED_OFF);
        }
        else if (strncmp(subject, "rbwC", 5UL) == 0)
        {
            ledCtrl->setLedMode(RAINBOW_CYCLE);
        }
        else if (strncmp(subject, "wRbw", 5UL) == 0)
        {
            ledCtrl->setLedMode(WHITE_OVER_RAINBOW);
        }
        else if (strncmp(subject, "rbwW", 5UL) == 0)
        {
            ledCtrl->setLedMode(RAINBOW_TO_WHITE);
        }
        else if (strncmp(subject, "wipe", 5UL) == 0)
        {
            ledCtrl->setLedMode(COLOR_WIPE);
        }
        else if (strncmp(subject, "white", 6UL) == 0)
        {
            ledCtrl->setLedMode(PULSE_WHITE);
        }
        else
        {
            rtnErr = ER_FAIL;
        }
    }

    return rtnErr;
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
