//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "LedModeCommand.hpp"

#include "LEDControl.hpp"
#include "MaintenanceComm.hpp"

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
    ErrorCode rtnErr = ER_FAIL;

    if (subject != NULL)
    {
        rtnErr = ER_SUCCESS;

        if (strncmp(subject, "off", 4UL) == 0)    // JSF151.1 Exception
        {
            ledCtrl->setLedMode(LED_OFF);
            maintComm->sendData("LED Mode -> OFF");
        }
        else if (strncmp(subject, "rbwC", 5UL) == 0)
        {
            ledCtrl->setLedMode(RAINBOW_CYCLE);
            maintComm->sendData("LED Mode -> Rainbow Cycle");
        }
        else if (strncmp(subject, "wRbw", 5UL) == 0)
        {
            ledCtrl->setLedMode(WHITE_OVER_RAINBOW);
            maintComm->sendData("LED Mode -> White over Rainbow");
        }
        else if (strncmp(subject, "rbwW", 5UL) == 0)
        {
            ledCtrl->setLedMode(RAINBOW_TO_WHITE);
            maintComm->sendData("LED Mode -> Rainbow to White");
        }
        else if (strncmp(subject, "wipe", 5UL) == 0)
        {
            ledCtrl->setLedMode(COLOR_WIPE);
            maintComm->sendData("LED Mode -> Color Wipe");
        }
        else if (strncmp(subject, "white", 6UL) == 0)
        {
            ledCtrl->setLedMode(PULSE_WHITE);
            maintComm->sendData("LED Mode -> White");
        }
        else
        {
            maintComm->sendData("Invalid LED mode. Acceptable modes:\r\n");
            maintComm->sendData("   off\r\n");
            maintComm->sendData("   rbwC\r\n");
            maintComm->sendData("   wRbw\r\n");
            maintComm->sendData("   rbwW\r\n");
            maintComm->sendData("   wipe\r\n");
            maintComm->sendData("   white\r\n");
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
