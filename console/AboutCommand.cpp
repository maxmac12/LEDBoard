//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "AboutCommand.hpp"
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

AboutCommand::AboutCommand()
{
    // do nothing
}


ErrorCode AboutCommand::exec(const CStr subject, const CStr value1, const CStr value2, const CStr value3)
{
    maintComm->sendData("LEDBoard Software\r\n");
    maintComm->sendData("Developed by Max MacCamy, 2018\r\n");
    maintComm->sendData("Supported Commands:\r\n");
    maintComm->sendData("   mode off\r\n");
    maintComm->sendData("   mode white\r\n");
    maintComm->sendData("   mode rainbow\r\n");
    maintComm->sendData("   mode wrainbow [wLength] [delay (ms)]\r\n");
    maintComm->sendData("   mode color [r] [g] [b]\r\n");
    maintComm->sendData("   mode pulse [delay (ms)]\r\n");
    maintComm->sendData("   stat\r\n");

    return (ER_SUCCESS);
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
