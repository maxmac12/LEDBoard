//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "Command.hpp"
#include "dataTypes.h"

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

Command::Command()
{
    // Do nothing.
}

//############################################################################
//  Protected Methods
//############################################################################

bool Command::subjectIsValid(const CStr subject) const
{
    return (subject != NULL);
}


bool Command::subjectAndValue1AreValid(const CStr subject,
                                       const CStr value1) const
{
    return (subjectIsValid(subject) && (value1 != NULL));
}


bool Command::allParamsAreValid(const CStr subject,
                                const CStr value1,
                                const CStr value2,
                                const CStr value3) const
{
    return (subjectAndValue1AreValid(subject, value1) && (value2 != NULL) && (value3 != NULL));
}

//############################################################################
//  Private Methods
//############################################################################
