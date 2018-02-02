#ifndef CONSOLE_CTRL_H
#define CONSOLE_CTRL_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "Task.hpp"
#include "dataTypes.h"
#include "error.hpp"
#include "Command.hpp"             // Command
#include "HashTable.hpp"           // HashTable
#include "MaintenanceComm.hpp"     // MaintenanceComm
#include "Singleton.hpp"
#include "StopWatch.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------
 
//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class ConsoleCtrl : public Task
{
    public:

        explicit ConsoleCtrl();

        void init(void);
        void exec(void);
        void halt(const Msec timeToSleep);

    protected:

    private:
        
        static const U32 MAX_NUM_CMDS = 64;   // Maximum number of commands.

        void processCommands(void);
        ErrorCode addCommand(const CStr cmdName, const CStr cmdClassName, Command* ptrCmdObj);

        U8 buf[MaintenanceComm::MAX_CONSOLE_LINE_LEN]; // Console input buffer.

        // hash table for commands - store the address of the command object
        HashTable<S32>* ptrHashCmds;
    
        StopWatch haltTimer;

        // Unused and disabled.
        ~ConsoleCtrl() {}
        ConsoleCtrl(const ConsoleCtrl&);
        ConsoleCtrl& operator=(const ConsoleCtrl&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------
#define consoleCtrl Singleton<ConsoleCtrl>::getInstance()

#endif
