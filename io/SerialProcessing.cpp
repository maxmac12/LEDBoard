//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SerialProcessing.hpp"
#include "SerialComm.hpp"

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

SerialProcessing::SerialProcessing() :
    Task(TASK_SERIAL_PROC_PERIOD,
         TASK_SERIAL_PROC_NAME,
         TID_SERIAL_PROC)
{
    // Do nothing
}


void SerialProcessing::init(void)
{
    // Do nothing.
}


void SerialProcessing::exec(void)
{
    // Attempt to get new RX message to process. These message has already been CRC verified.
    if (serialComm->receiveData(buf.data) == ER_SUCCESS)
    {
        DataHeader::Command command = (reinterpret_cast<const DataHeader* const>(buf.data)->command);

        switch(command)
        {
            case DataHeader::MODE_CMD:
                ledCtrl->setLedMode((reinterpret_cast<const ModeCommand* const>(buf.data)->mode));
                break;

            case DataHeader::COLOR_CMD:
                break;

            case DataHeader::SPECTRUM_CMD:
                break;

            default:
                break;
        }
    }
}


//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

