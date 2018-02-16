#ifndef LED_MODE_COMMAND_H
#define LED_MODE_COMMAND_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "Command.hpp"    // base class
#include "dataTypes.h"
#include "error.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------
 
//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class LedModeCommand : public Command
{
    public:

        explicit LedModeCommand();

        ErrorCode exec(const CStr subject, const CStr value1, const CStr value2, const CStr value3);

    protected:

    private:
        
        // Unused and disabled.
        ~LedModeCommand() {}
        LedModeCommand(const LedModeCommand&);
        LedModeCommand& operator=(const LedModeCommand&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------

#endif
