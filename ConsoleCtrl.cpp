//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "ConsoleCtrl.hpp"
#include "Task.hpp"   // base class
#include "appconfig.hpp"
#include "dataTypes.h"
#include "stringUtils.h"
#include "AboutCommand.hpp"
#include "LedModeCommand.hpp"
#include "StatCommand.hpp"
#include "CommonData.hpp"
#include "MaintenanceComm.hpp"
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

struct CommandList
{
    const CStr commandName;
    const CStr commandClass;
    Command* ptrCommandObj;
};

// list of commands in the system
CommandList cmdList[] =
{
    // *** ADD NEW COMMANDS HERE ***
    { "about", "AboutCommand", new AboutCommand() },
    { "mode", "LedModeCommand", new LedModeCommand() },
    { "stat", "StatCommand", new StatCommand() },

    { NULL, NULL, NULL }    // end of list - DO NOT REMOVE!!!
};

//############################################################################
//  Public Methods
//############################################################################

ConsoleCtrl::ConsoleCtrl() :
    Task(TASK_CONSOLE_PERIOD,
         TASK_CONSOLE_NAME,
         TID_CONSOLE_CTRL),
    ptrHashCmds(new HashTable<S32>(MAX_NUM_CMDS))
{
    if (ptrHashCmds == NULL)
    {
        disableTask(ER_CONSOLE_INIT);
    }

    haltTimer.start(0);  // initialize to 'expired' timer
}


void ConsoleCtrl::init(void)
{
    // add all console commands to the internal command list
    for (U8 cmdIdx = 0; cmdList[cmdIdx].commandName != NULL; cmdIdx++)
    {
        if (cmdList[cmdIdx].ptrCommandObj == NULL)
        {
            disableTask(ER_CONSOLE_INIT);
        }

        if (addCommand(cmdList[cmdIdx].commandName,
                       cmdList[cmdIdx].commandClass,
                       cmdList[cmdIdx].ptrCommandObj) != ER_SUCCESS)
        {
            // do nothing
        }
    }

    // write prompt
    maintComm->sendData("\ntnsy$ ", false);
}


void ConsoleCtrl::exec(void)
{
    if (haltTimer.timerHasExpired())
    {
        processCommands();
    }
}


void ConsoleCtrl::halt(const Msec timeToSleep)
{
    haltTimer.start(timeToSleep);
}

//############################################################################
//  Protected Methods
//############################################################################

ErrorCode ConsoleCtrl::addCommand(const CStr cmdName,
                                  const CStr cmdClassName,
                                  Command* ptrCmdObj)
{
    // treat the Command pointer as a 32-bit integer
    return (((ptrHashCmds != NULL) && (ptrCmdObj != NULL)) ?
        ptrHashCmds->insert(cmdName, (new CommonData<S32>(
            cmdClassName,
            reinterpret_cast<S32>(ptrCmdObj),
            0x00000000)
        )) : ER_HASH_TABLE_SETUP
    );
}


void ConsoleCtrl::processCommands(void)
{
    if (maintComm->receiveData(buf) == ER_SUCCESS)
    {
        // tokenize the string - 4 times the CDS entry max limit for strings ought to be enough
        CStr ptrBuf  = reinterpret_cast<CStr>(buf);
        CStr command = strnsep(&ptrBuf, " ", CommonData<S32>::MAX_NAME_LEN * 4UL, 2UL);  // command
        CStr subject = strnsep(&ptrBuf, " ", CommonData<S32>::MAX_NAME_LEN * 4UL, 2UL);  // subject
        CStr value1  = strnsep(&ptrBuf, " ", CommonData<S32>::MAX_NAME_LEN * 4UL, 2UL);  // 1st value
        CStr value2  = strnsep(&ptrBuf, " ", CommonData<S32>::MAX_NAME_LEN * 4UL, 2UL);  // 2nd value
        CStr value3  = strnsep(&ptrBuf, " ", CommonData<S32>::MAX_NAME_LEN * 4UL, 2UL);  // 3rd value

        if (ptrHashCmds != NULL)
        {
            CommonData<S32>* ptrData = NULL;

            if (ptrHashCmds->find(command, ptrData) == ER_SUCCESS)  // 'command' won't be NULL because ptrBuf is not
            {
                maintComm->sendData("\r\n", false);

                // execute the command if command is found
                if (reinterpret_cast<Command*>(ptrData->get())->exec(
                        subject, value1, value2, value3) != ER_SUCCESS)
                {
                    maintComm->sendData("\r\nCommand could not be executed.");
                }
            }
            // any input other than a carriage return is invalid
            else if (*command != NUL)
            {
                maintComm->sendData("\r\nInvalid command.");
            }

            // write prompt
            maintComm->sendData("\r\ntnsy$ ", false);
        }
    }
}

//############################################################################
//  Private Methods
//############################################################################
