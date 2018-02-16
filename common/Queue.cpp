//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "Queue.hpp"
#include "dataTypes.h"
#include "error.hpp"
#include "string.h"         // memcpy()

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

Queue::Queue(const U32 newQueueDepth, U32 const newDataSize, const QueueIDEnum newQueueID) :
    numItems(0),
    queueDepth(newQueueDepth),
    queueDataSize(newDataSize),
    lastSlot(0),
    queueID(newQueueID)
{
    ptrQueue = new U8[newQueueDepth * newDataSize];  // Allocate queue space.
    ptrFront = &ptrQueue[0];
    ptrBack  = &ptrQueue[0];

    lastSlot = (newQueueDepth - 1) * newDataSize;

    if (NULL == ptrQueue)
    {
        queueDepth = 0;
    }
}


U32 Queue::getNumItems(void) const
{
    return numItems;
}


U32 Queue::getQueueDepth(void) const
{
    return queueDepth;
}

bool Queue::isEmpty(void) const
{
    return (numItems == 0UL);
}


ErrorCode Queue::pop(void* const ptrData)
{
    ErrorCode err = ER_SUCCESS;

    if (isEmpty())
    {
        err = ER_QUEUE_EMPTY;
    }
    else
    {
        if (ptrData)
        {
            // copy data back
            memcpy(ptrData, ptrFront, queueDataSize);
            numItems--;

            if (ptrFront == &ptrQueue[lastSlot])
            {
                // Reached the end of the queue. Wrap around to the beginning of the queue.
                ptrFront = &ptrQueue[0];
            }
            else
            {
                // Increment pointer to the next slot in the queue.
                ptrFront += queueDataSize;
            }
        }
        else
        {
            err = ER_FAIL;
        }
    }

    return (err);
}


ErrorCode Queue::push(const void* const ptrData)
{
    ErrorCode err = ER_SUCCESS;

    if (ptrData)
    {
        if (numItems < queueDepth)
        {
            // Still space in queue.
            memcpy(ptrBack, ptrData, queueDataSize);
            numItems++;

            if (ptrBack == &ptrQueue[lastSlot])
            {
                // Reached the end of the queue. Wrap around to the beginning of the queue.
                ptrBack = &ptrQueue[0];
            }
            else
            {
                // Increment pointer to the next slot in the queue.
                ptrBack += queueDataSize;
            }
        }
        else
        {
            err = ER_QUEUE_FULL;
        }
    }
    else
    {
        err = ER_FAIL;
    }

    return (err);
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
