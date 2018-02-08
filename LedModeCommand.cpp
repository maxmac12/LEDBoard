//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "LedModeCommand.hpp"
#include "LEDControl.hpp"
#include "MaintenanceComm.hpp"
#include <WCharacter.h>
#include <stdio.h>
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

    if (subjectIsValid(subject))
    {
        S8 buf[MaintenanceComm::MAX_CONSOLE_LINE_LEN];  // Buffer for formatting strings.
        rtnErr = ER_SUCCESS;

        if (strncmp(subject, "off", 4UL) == 0)
        {
            ledCtrl->turnLedsOff();
            maintComm->sendData("LEDs OFF");
        }
        else if (strncmp(subject, "rainbow", 8UL) == 0)
        {
            ledCtrl->setLedMode(RAINBOW_CYCLE);
            maintComm->sendData("LED Mode -> Rainbow Cycle");
        }
        else if (strncmp(subject, "wrainbow", 9UL) == 0)
        {
            // Check that options are valid ASCII digit and aren't empty.
            if ((isAscii(*value1) && (strncmp(value1, "", 1UL) != 0)) &&
                (isAscii(*value2) && (strncmp(value2, "", 1UL) != 0)))
            {
                // Convert options to integers.
                U32 length = strtoul(value1, NULL, DEC);
                Msec speed = strtoul(value2, NULL, DEC);

                ledCtrl->setWhiteRainbowLength(length);
                ledCtrl->setWhiteRainbowSpeed(speed);
            }

            ledCtrl->setLedMode(WHITE_OVER_RAINBOW);
            maintComm->sendData("LED Mode -> White Over Rainbow");
        }
        else if (strncmp(subject, "color", 6UL) == 0)
        {
            rtnErr = ER_FAIL;

            // Check that options are valid ASCII digit and aren't empty.
            if ((isAscii(*value1) && (strncmp(value1, "", 1UL) != 0)) &&
                (isAscii(*value2) && (strncmp(value2, "", 1UL) != 0)) &&
                (isAscii(*value3) && (strncmp(value3, "", 1UL) != 0)))
            {
                // Convert options to integers.
                U32 red   = strtoul(value1, NULL, DEC);
                U32 green = strtoul(value2, NULL, DEC);
                U32 blue  = strtoul(value3, NULL, DEC);

                ledCtrl->setLedMode(COLOR);
                ledCtrl->setCurrentColor(red, green, blue);
                snprintf(buf, MaintenanceComm::MAX_CONSOLE_LINE_LEN, "Updated Color (r: %d, g: %d, b: %d)", red, green, blue);
                maintComm->sendData(buf);
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
        else if (strncmp(subject, "pulse", 6UL) == 0)
        {
            // Check that options are valid ASCII digit and aren't empty.
            if (isAscii(*value1) && (strncmp(value1, "", 1UL) != 0))
            {
                // Convert option to integer.
                Msec speed = strtoul(value1, NULL, DEC);

                ledCtrl->setPulseSpeed(speed);
            }

            ledCtrl->setLedMode(COLOR_PULSE);
            maintComm->sendData("LED Mode -> Pulse Color");
        }
        else if (strncmp(subject, "white", 6UL) == 0)
        {
            ledCtrl->setLedMode(COLOR);
            ledCtrl->setCurrentColor(255, 255, 150);
            maintComm->sendData("White Light ON");
        }
        else
        {
            maintComm->sendData("Invalid LED mode. Acceptable modes:\r\n");
            maintComm->sendData("   off\r\n");
            maintComm->sendData("   rainbow\r\n");
            maintComm->sendData("   wrainbow [wLength] [delay (ms)]\r\n");
            maintComm->sendData("   color [r] [g] [b]\r\n");
            maintComm->sendData("   bright [0-100]\r\n");
            maintComm->sendData("   pulse [delay (ms)]\r\n");
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
