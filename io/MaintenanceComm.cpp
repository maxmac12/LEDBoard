//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
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

MaintenanceComm::MaintenanceComm() :
    Task(TASK_MAINT_PERIOD,
         TASK_MAINT_NAME,
         TID_MAINT_COMM),
    currLineLoc(0),
    rxQueue(new Queue(RX_QUEUE_LEN, RX_DATA_LEN, QID_MAINT_RX)),
    txQueue(new Queue(TX_QUEUE_LEN, TX_DATA_LEN, QID_MAINT_TX))
{
    // do nothing
}


void MaintenanceComm::init(void)
{
    DEBUG_PORT.begin(DEBUG_BAUD);  // Teensy 3.6 USB always runs at 12 Mbit/sec

    while(!DEBUG_PORT);
    DEBUG_PORT.println("Maintenance Port Initiated");
}


void MaintenanceComm::exec(void)
{
    process();
}


void MaintenanceComm::sendData(const S8* ptrVal, const bool newLine, const bool immediate)
{
    if (!immediate)
    {
        // arbitrarily limit input to 32 * 128 characters
        U32 len = static_cast<U32>(strnlen(ptrVal, MAX_CONSOLE_LINE_LEN * 32UL));

        if (len >= MAX_CONSOLE_LINE_LEN)
        {
            // split up output into chunks that fit into the buffer
            while (len >= (MAX_CONSOLE_LINE_LEN - 1UL))
            {
                strncpy(txBuf, ptrVal, MAX_CONSOLE_LINE_LEN - 1UL);
                txBuf[MAX_CONSOLE_LINE_LEN - 1UL] = '\0';   // termination
                static_cast<void>(txQueue->push(txBuf));

                // go to the next portion of the string
                ptrVal += (MAX_CONSOLE_LINE_LEN - 1UL);
                len -= (MAX_CONSOLE_LINE_LEN - 1UL);
            }
        }

        // send the remaining bytes
        if (len > 0UL)
        {
            snprintf(txBuf, MAX_CONSOLE_LINE_LEN, newLine ? "%s\n" : "%s", ptrVal);
            static_cast<void>(txQueue->push(txBuf));
        }
    }
    else
    {
        // no buffering desired; output immediately
        DEBUG_PORT.printf(newLine ? "%s\n" : "%s", ptrVal);
    }
}


void MaintenanceComm::sendData(const S32 size, const S8* const ptrFmt, ...)
{
    va_list args;

    va_start(args, ptrFmt);
    vsnprintf(buf, static_cast<U32>(size), ptrFmt, args);
    va_end(args);

    sendData(buf);
}


ErrorCode MaintenanceComm::receiveData(void* ptrBlockData)
{
    return (rxQueue->pop(ptrBlockData));
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void MaintenanceComm::process(void)
{
    receive();
    send();
}


void MaintenanceComm::send(void)
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
            DEBUG_PORT.printf("%s", txBuf);
        }
    }
}


void MaintenanceComm::receive(void)
{
    // read incoming data
    U8 byte = 0x00;
    bool readFinished = false;  // read one command at a time

    // read characters until one complete command has been read
    while (!readFinished)
    {
        // Check if there are bytes in the receive buffer.
        if (DEBUG_PORT.available() > 0)
        {
            byte = DEBUG_PORT.read();

            // look for carriage return, line feed, or max line length
            if ((CARRIAGE_RETURN == byte) ||
                (currLineLoc == MAX_CONSOLE_LINE_LEN))
            {
                rxBuf[(currLineLoc == MAX_CONSOLE_LINE_LEN) ?
                     MAX_CONSOLE_LINE_LEN - 1UL : currLineLoc] = '\0';     // null-terminate the string
                currLineLoc = 0;

                // stick the command into the queue
                if (rxQueue->push(rxBuf) != ER_SUCCESS)
                {
                    // do nothing
                }

                readFinished = true;
            }
            else if (byte == BACKSPACE)  // backspace
            {
                if (currLineLoc > 0UL)
                {
                    --currLineLoc;

                    // to simulate a backspace on the console, send backspace, space, and backspace in
                    // sequence to simulate a destructive backspace keystroke
                    U8 keystrokes[] = { BACKSPACE, SPACE, BACKSPACE };

                    for (U8 key = 0; key < sizeof(keystrokes); key++)
                    {
                        DEBUG_PORT.print((char)keystrokes[key]);
                    }
                }
            }
            else
            {
                // save the current byte read
                rxBuf[currLineLoc++] = byte;

                // echo the input
                DEBUG_PORT.print((char)byte);
            }
        }
        else
        {
            // Receive buffer is empty.
            readFinished = true;
        }
    }
}
