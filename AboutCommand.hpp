#ifndef ABOUT_COMMAND_H
#define ABOUT_COMMAND_H

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

class AboutCommand : public Command
{
    public:

        explicit AboutCommand();

        ErrorCode exec(const CStr subject, const CStr value1, const CStr value2);

    protected:

    private:
        
        // Unused and disabled.
        ~AboutCommand() {}
        AboutCommand(const AboutCommand&);
        AboutCommand& operator=(const AboutCommand&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------

#endif
