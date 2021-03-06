//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "SerialProcessing.hpp"
#include "SerialComm.hpp"
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

        switch (command)
        {
            case DataHeader::MODE_CMD:
            {
                ledCtrl->setLedMode((reinterpret_cast<const ModeCommand* const>(buf.data)->mode));
                break;
            }

            case DataHeader::COLOR_CMD:
            {
                const ColorCommand* msg = (reinterpret_cast<const ColorCommand* const>(buf.data));
                ledCtrl->setCurrentColor(msg->red, msg->green, msg->blue);
                ledCtrl->setLedMode(COLOR);
                break;
            }

            case DataHeader::SPECTRUM_CMD:
            {
                const SpectrumCommand* msg = (reinterpret_cast<const SpectrumCommand* const>(buf.data));

                for (U8 i = 0; i < NUM_LED_STRIPS; i++)
                {
                    ledCtrl->setStripHeight(i, msg->ledHeight[i]);
                }

                ledCtrl->setLedMode(SPECTRUM);
                break;
            }

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

