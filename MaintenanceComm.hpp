#ifndef MAINTENANCE_COMM_H
#define MAINTENANCE_COMM_H

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

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

#define COM_PORT   Serial5
#define COM_BAUD   115200
#define COM_RX_PIN A15
#define COM_TX_PIN A14

static const U8 CARRIAGE_RETURN = 0x0D;     // Carriage return.
static const U8 LINE_FEED       = 0x0A;     // Line Feed.
static const U8 BACKSPACE       = 0x08;     // Backspace.
static const U8 SPACE           = 0x20;     // Space.

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------
 
//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class MaintenanceComm : public Task
{
    public:

        // Maximum console line length.
        static const U32 MAX_CONSOLE_LINE_LEN = 128;

        explicit MaintenanceComm();

        void init(void);
        void exec(void);
        void sendData(const S8* ptrVal, const bool newLine = true, const bool immediate = false);
        void sendData(const S32 size, const S8* const ptrFmt, ...);
        ErrorCode receiveData(void* ptrBlockData);

    protected:

    private:

        // Queue Depths
        static const U32 RX_QUEUE_LEN = 64;  // RX queue depth.
        static const U32 TX_QUEUE_LEN = 64;  // TX queue depth.
		
		// Queue Data Lengths
        static const U32 RX_DATA_LEN = MAX_CONSOLE_LINE_LEN;  // Rx queue data length
		static const U32 TX_DATA_LEN = MAX_CONSOLE_LINE_LEN;  // Tx queue data length

        void process(void);
        void send(void);
        void receive(void);

        S8 rxBuf[MAX_CONSOLE_LINE_LEN];  // Buffer for receiving bytes.
        S8 txBuf[MAX_CONSOLE_LINE_LEN];  // Buffer for sending bytes.
        S8 buf[MAX_CONSOLE_LINE_LEN];    // General use buffer for constructing dynamic strings.

        U32 currLineLoc;  // Keep track of console location.

        Queue* rxQueue;   // RX queue.
        Queue* txQueue;   // TX queue.

        // Unused and disabled.
        ~MaintenanceComm() {}
        MaintenanceComm(const MaintenanceComm&);
        MaintenanceComm& operator=(const MaintenanceComm&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------
#define maintComm Singleton<MaintenanceComm>::getInstance()

#endif
