#ifndef COMMAND_H
#define COMMAND_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: Command
// 
// Purpose:
//      Interface to command classes.
//
// Notes:
//      Console commands are in the form of:
//          [command] [subject] [value1] [value2]
// 
//      Subclasses are free to use or disregard the subject, value1, and value2
//      parameters.
//
//----------------------------------------------------------------------------
class Command
{
    public:         

        //--------------------------------------------------------------------
        // Purpose:
        //     Constructor.
        //
        // Parameters:
        //     None.
        // 
        // Return:
        //     N/A.
        // 
        // Notes:
        //     None.
        //--------------------------------------------------------------------        
        explicit Command();

        //--------------------------------------------------------------------
        // Purpose:
        //     Command execution.
        //
        // Parameters:
        //     [in] subject - Subject of the command.
        //     [in] value1 - The first value parameter of the command.
        //     [in] value2 - The second value parameter of the command.
        //     [in] value3 - The second value parameter of the command.
        // 
        // Return:
        //     Dependent on implementation.
        // 
        // Notes:
        //     To be implemented by derived classes.
        //--------------------------------------------------------------------        
        virtual ErrorCode exec(const CStr subject, const CStr value1, const CStr value2, const CStr value3) = 0;

        // Unused and disabled.
        virtual ~Command() {}

    protected:

        //--------------------------------------------------------------------
        // Purpose:
        //     Check for command parameter validity.
        //
        // Parameters:
        //     [in] subject - Subject of the command.
        // 
        // Return:
        //     True if the subject is a valid string.
        //     False if the subject is not a valid string.
        // 
        // Notes:
        //     None.
        //--------------------------------------------------------------------        
        bool subjectIsValid(const CStr subject) const;

        //--------------------------------------------------------------------
        // Purpose:
        //     Check for command parameter validity.
        //
        // Parameters:
        //     [in] subject - Subject of the command.
        //     [in] value1 - The first value parameter of the command.
        // 
        // Return:
        //     True if both the subject and the first value parameter are 
        //         valid strings.
        //     False if either the subject or the first value parameter are
        //         not valid strings.
        // 
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        bool subjectAndValue1AreValid(const CStr subject,
                                      const CStr value1) const;

        //--------------------------------------------------------------------
        // Purpose:
        //     Check for command parameter validity.
        //
        // Parameters:
        //     [in] subject - Subject of the command.
        //     [in] value1 - The first value parameter of the command.
        //     [in] value2 - The second value parameter of the command.
        // 
        // Return:
        //     True if all input parameters are valid strings.
        //     False if any one of the input parameters is not a valid string.
        // 
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        bool allParamsAreValid(const CStr subject,
                               const CStr value1,
                               const CStr value2) const;
                               
    private:
            
        // Unused and disabled.
        Command(const Command&);
        Command& operator=(const Command&);
};

//--------------------------------------------------------------------
//  Global Definitions 
//--------------------------------------------------------------------

#endif
