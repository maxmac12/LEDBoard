//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "LedModeCommand.hpp"
#include "LEDControl.hpp"
#include "MaintenanceComm.hpp"
#include <WCharacter.h>
#include <stdlib.h>
#include <Print.h>

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


ErrorCode LedModeCommand::exec(const CStr subject, const CStr value1, const CStr value2, CStr value3)
{
    ErrorCode rtnErr = ER_FAIL;

    if (subject != NULL)
    {
        rtnErr = ER_SUCCESS;

        if (strncmp(subject, "off", 4UL) == 0)
        {
            ledCtrl->setLedMode(LED_OFF);
            maintComm->sendData("LED Mode -> OFF");
        }
        else if (strncmp(subject, "rainbow", 8UL) == 0)
        {
            ledCtrl->setLedMode(RAINBOW_CYCLE);
            maintComm->sendData("LED Mode -> Rainbow Cycle");
        }
        else if (strncmp(subject, "wrainbow", 9UL) == 0)
        {
            // Check to see if first character is a valid number and not NUL (limit to 8 bit)
            if (isAscii(*value1) && (strncmp(value1, NUL, 2UL) != 0))
            {
                // Convert options to integers.
                U32 length = strtoul(value1, NULL, DEC);
                ledCtrl->setWhiteRainbowLength(length);
            }

            if (isAscii(*value2) && (strncmp(value2, NUL, 2UL) != 0))
            {
                Msec speed = strtoul(value2, NULL, DEC);
                ledCtrl->setWhiteRainbowSpeed(speed);
            }

            ledCtrl->setLedMode(WHITE_OVER_RAINBOW);
            maintComm->sendData("LED Mode -> White over Rainbow");
        }
        else if (strncmp(subject, "color", 6UL) == 0)
        {
            rtnErr = ER_FAIL;

            // Check to see if first character is a valid number (limit to 8 bit)
            if (isAscii(*value1) && isAscii(*value2) && isAscii(*value3))
            {
                // Convert options to integers.
                U32 red   = strtoul(value1, NULL, DEC);
                U32 green = strtoul(value2, NULL, DEC);
                U32 blue  = strtoul(value3, NULL, DEC);

                ledCtrl->setLedMode(COLOR, ledCtrl->getColor(red, green, blue));
                maintComm->sendData("LED Mode -> Color");
                rtnErr = ER_SUCCESS;
            }
        }
        else if (strncmp(subject, "bright", 7UL) == 0)
        {
            rtnErr = ER_FAIL;

            // Check to see if first character is a valid number (limit to 8 bit)
            if (isAscii(*value1))
            {
                // Convert options to integers.
                U32 brightness   = strtoul(value1, NULL, DEC);

                ledCtrl->setBrightness(brightness);
                maintComm->sendData("LED Brightness Updated");
                rtnErr = ER_SUCCESS;
            }
        }
        else if (strncmp(subject, "wipe", 5UL) == 0)
        {
            ledCtrl->setLedMode(COLOR_WIPE);
            maintComm->sendData("LED Mode -> Color Wipe");
        }
        else if (strncmp(subject, "pwhite", 7UL) == 0)
        {
            ledCtrl->setLedMode(PULSE_WHITE);
            maintComm->sendData("LED Mode -> Pulse White");
        }
        else if (strncmp(subject, "white", 6UL) == 0)
        {
            ledCtrl->setLedMode(WHITE);
            maintComm->sendData("LED Mode -> White");
        }
        else
        {
            maintComm->sendData("Invalid LED mode. Acceptable modes:\r\n");
            maintComm->sendData("   off\r\n");
            maintComm->sendData("   rainbow\r\n");
            maintComm->sendData("   wrainbow\r\n");
            maintComm->sendData("   color [r] [g] [b]\r\n");
            maintComm->sendData("   bright [value]\r\n");
            maintComm->sendData("   wipe\r\n");
            maintComm->sendData("   pwhite\r\n");
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
