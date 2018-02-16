#ifndef STAT_COMMAND_H
#define STAT_COMMAND_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "Command.hpp"
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

class StatCommand : public Command
{
    public:         

        explicit StatCommand();
        ErrorCode exec(const CStr subject, const CStr value1, const CStr value2, const CStr value3);

    protected:

    private:
        
        static const U8 OUTPUT_BUF_LEN = 64;    // Length of output buffer.

        // Unused and disabled.
        ~StatCommand() {}
        StatCommand(const StatCommand&);
        StatCommand& operator=(const StatCommand&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------

#endif
