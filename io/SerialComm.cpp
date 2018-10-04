//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "SerialComm.hpp"
#include "LEDControl.hpp"
#include "MaintenanceComm.hpp"
#include "appconfig.hpp"
#include "dataTypes.h"
#include "stringUtils.h"  // strncpy(), strnlen()
#include "Queue.hpp"
#include <HardwareSerial.h>
#include <stdarg.h>       // va_list, va_start(), etc.
#include <stdio.h>        // snprintf(), vsnprintf()


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

SerialComm::SerialComm() :
    Task(TASK_SERIAL_PERIOD,
         TASK_SERIAL_NAME,
         TID_SERIAL_COMM),
    rxQueue(new Queue(RX_QUEUE_LEN, RX_DATA_LEN, QID_MAINT_RX)),
    txQueue(new Queue(TX_QUEUE_LEN, TX_DATA_LEN, QID_MAINT_TX))
{
    // do nothing
}


void SerialComm::init(void)
{
    COM_PORT.begin(COM_BAUD);
    COM_PORT.setRX(COM_RX_PIN);
    COM_PORT.setTX(COM_TX_PIN);

    while(!COM_PORT);
}


void SerialComm::exec(void)
{
    process();
}

void SerialComm::sendData(const U8* ptrData)
{
    // TODO: Implement.
}


ErrorCode SerialComm::receiveData(void* ptrBlockData)
{
    return (rxQueue->pop(ptrBlockData));
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void SerialComm::process(void)
{
    receive();
    send();
}


void SerialComm::send(void)
{
    // send out outgoing data
    while (!txQueue->isEmpty())
    {
        if (txQueue->pop(txBuf) != ER_SUCCESS)
        {
            // do nothing
        }
        else
        {
            COM_PORT.write(txBuf, 1);  // TODO: Implement.
        }
    }
}


void SerialComm::receive(void)
{
    // read incoming data
    U8 byte = 0xFF;
    bool readFinished = false;  // read one command at a time

    // read characters until one complete command has been read
    while (!readFinished)
    {
        // Check if there are bytes in the receive buffer.
        if (COM_PORT.available() > 0)
        {
            byte = COM_PORT.read();

            // Look for different commands.
            if (OFF == byte)
            {
                ledCtrl->setLedMode(OFF);
            }
            else if (WHITE == byte)
            {
                ledCtrl->setLedMode(WHITE);
            }
            else if (COLOR == byte)
            {
                ledCtrl->setLedMode(COLOR);
            }
            else if (COLOR_PULSE == byte)
            {
                ledCtrl->setLedMode(COLOR_PULSE);
            }
            else if (RAINBOW_CYCLE == byte)
            {
                ledCtrl->setLedMode(RAINBOW_CYCLE);
            }
            else if (WHITE_OVER_RAINBOW == byte)
            {
                ledCtrl->setLedMode(WHITE_OVER_RAINBOW);
            }
            else
            {
                maintComm->sendData("\r\nInvalid Serial Rx.");
            }
        }
        else
        {
            // Receive buffer is empty.
            readFinished = true;
        }
    }
}
