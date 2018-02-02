#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "dataTypes.h"
#include "error.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------
 
//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class MemoryBlock
{
    public:

        explicit MemoryBlock(const void* const ptrNewData, const U32 newSize);

        void* get(void) const;
        ErrorCode set(const void* const ptrNewData, const U32 newSize);
        U32 getSize(void) const;
        void resize(const U32 newDataSize);

    protected:
    
    private:

        void* ptrData;  // Pointer to the data buffer.
        U32 size;       // Size of the buffer in bytes.
        U32 maxSize;    // Original size of the buffer allocation. Subsequent resizes cannot exceed this size.

        // Unused and disabled.
        ~MemoryBlock() {}
        MemoryBlock(const MemoryBlock&);
        MemoryBlock& operator=(const MemoryBlock&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------

typedef MemoryBlock* MemPtr;

#endif
