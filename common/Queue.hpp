#ifndef QUEUE_H
#define QUEUE_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "appconfig.hpp"
#include "dataTypes.h"
#include "error.hpp"
#include <string.h>

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class Queue
{
    public:

        explicit Queue(const U32 newQueueDepth, const U32 newDataSize, const QueueIDEnum queueID);

        bool isEmpty(void) const;
        U32 getNumItems(void) const;
        U32 getQueueDepth(void) const;
        ErrorCode pop(void* const ptrData);
        ErrorCode push(const void* ptrData);

        // Unused and disabled.
        ~Queue() {}

    protected:

    private:

        U8* ptrQueue;   // Pointer to the created queue buffer.
        U8* ptrFront;   // Pointer to the next item to be popped
        U8* ptrBack;    // Pointer to the last item to be pushed

        U32 numItems;         // Current number of items in queue.
        U32 queueDepth;       // Number of slots in queue.
        U32 queueDataSize;    // Size of data in queue slot.
        U32 lastSlot;         // Last slot in queue buffer.

        QueueIDEnum queueID;  // The Id of the queue for error reporting purposes.

        // Unused and disabled.
        Queue(const Queue&);
        Queue& operator=(const Queue&);
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#endif
