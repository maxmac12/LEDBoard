//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "SerialComm.hpp"
#include "LEDControl.hpp"
#include "MaintenanceComm.hpp"
#include "appconfig.hpp"
#include "crc.hpp"
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
    receive();
    send();
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
    // Read incoming data
    static bool msgReceived = false;
    static U8   msgDataEndIndex = 0;
    static U16  bytesRead = 0;

    U8 byte = 0x00;

    while ((COM_PORT.available() > 0) && !msgReceived)
    {
        // buffer the incoming bytes
        byte = (U8)COM_PORT.read();

        maintComm->sendData(MaintenanceComm::MAX_CONSOLE_LINE_LEN, "State: %d, RX: 0x%X, BRead: %d", rxState, byte, bytesRead, true);

        switch(rxState)
        {
            case GET_PREAMBLE:
                if (0xEE == byte)
                {
                    buf.data[bytesRead++] = byte;
                    rxState = GET_LENGTH;
                }
                break;

            case GET_LENGTH:
                buf.data[bytesRead++] = byte;
                msgDataEndIndex = bytesRead + byte;
                maintComm->sendData(MaintenanceComm::MAX_CONSOLE_LINE_LEN, "msgDataEndIndex: %d", msgDataEndIndex, true);
                rxState = GET_COMMAND;
                break;

            case GET_COMMAND:
                buf.data[bytesRead++] = byte;
                rxState = GET_DATA;
                break;

            case GET_DATA:
                buf.data[bytesRead++] = byte;

                if (bytesRead > msgDataEndIndex)
                {
                    rxState = GET_CRC_LSB;
                }
                break;

            case GET_CRC_LSB:
                buf.data[bytesRead++] = byte;
                rxState = GET_CRC_MSB;
                break;

            case GET_CRC_MSB:
                buf.data[bytesRead++] = byte;
                rxState = GET_PREAMBLE;
                msgDataEndIndex = 0;
                msgReceived = true;
                break;

            default:
                rxState = GET_PREAMBLE;
                msgDataEndIndex = 0;
                bytesRead = 0;
                msgReceived = false;
                break;
        }

        if (bytesRead > MAX_SERIAL_PACKET_LEN)
        {
            maintComm->sendData("Rx Message Length Invalid");
            rxState = GET_PREAMBLE;
            msgDataEndIndex = 0;
            bytesRead = 0;
            msgReceived = false;
            break;
        }
    }

    if (msgReceived)
    {
        maintComm->sendData("RX Message", true);

        // Check CRC of the received data and put data into queue
        if ((bytesRead > sizeof(U16)) && (bytesRead <= MAX_SERIAL_PACKET_LEN))
        {
            U16 expected_crc = *(reinterpret_cast<U16*>(&(buf.data[bytesRead - sizeof(U16)])));
            U16 calculated_crc = crc16(buf.data, bytesRead - sizeof(U16));

            maintComm->sendData(MaintenanceComm::MAX_CONSOLE_LINE_LEN,
                               "RX Bytes = %d", bytesRead, true);

            if (calculated_crc == expected_crc)
            {
                maintComm->sendData("RX Success", true);

                // stuff the bytes into the queue
                if (rxQueue->push(buf.data) != ER_SUCCESS)
                {
                    // TODO: Do something.
                    maintComm->sendData("Rx Push Fail", true);
                }
            }
            else
            {
                maintComm->sendData(MaintenanceComm::MAX_CONSOLE_LINE_LEN,
                                   "RX CRC Bad: 0x%X != 0x%X", calculated_crc, expected_crc, true);
            }
        }

        msgReceived = false;
        bytesRead = 0;
    }
}
