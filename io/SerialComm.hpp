#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "dataTypes.h"
#include "error.hpp"
#include "Queue.hpp"
#include "Singleton.hpp"
#include "Task.hpp"

#include <HardwareSerial.h>
#include <pins_arduino.h>
#include <usb_serial.h>

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

#define COM_PORT    Serial5
#define COM_BAUD    500000
#define COM_RX_PIN  A15
#define COM_TX_PIN  A14

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------
 
//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class SerialComm : public Task
{
    public:

        // Maximum serial message length.
        static const U32 MAX_SERIAL_MSG_LEN = 128;  // TODO: Determine actual length.

        explicit SerialComm();

        void init(void);
        void exec(void);
        void sendData(const U8* ptrData);
        ErrorCode receiveData(void* ptrBlockData);

    protected:

    private:

        // Queue Depths
        static const U32 RX_QUEUE_LEN = 64;  // RX queue depth.
        static const U32 TX_QUEUE_LEN = 64;  // TX queue depth.

        // Queue Data Lengths
        static const U32 RX_DATA_LEN = MAX_SERIAL_MSG_LEN;  // Rx queue data length
        static const U32 TX_DATA_LEN = MAX_SERIAL_MSG_LEN;  // Tx queue data length

        void process(void);
        void send(void);
        void receive(void);

        U8 rxBuf[MAX_SERIAL_MSG_LEN];  // Buffer for receiving bytes.
        U8 txBuf[MAX_SERIAL_MSG_LEN];  // Buffer for sending bytes.

        Queue* rxQueue;   // RX queue.
        Queue* txQueue;   // TX queue.

        // Unused and disabled.
        ~SerialComm() {}
        SerialComm(const SerialComm&);
        SerialComm& operator=(const SerialComm&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------
#define serialComm Singleton<SerialComm>::getInstance()

#endif
