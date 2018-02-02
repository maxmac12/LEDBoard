//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "MemoryBlock.hpp"
#include "error.hpp"
#include "string.h"          // memcpy()

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

MemoryBlock::MemoryBlock(const void* const ptrNewData, const U32 newSize) :

    // if no incoming default data, create new memory buffer; otherwise, just
    // point to the existing memory buffer

    ptrData(((ptrNewData == NULL) ? new U8[newSize] : const_cast<void*>(ptrNewData)))
{
    // check allocated memory
    size = (ptrData == NULL) ? 0 : newSize;
    maxSize = size;
}


void* MemoryBlock::get(void) const
{
    return (ptrData);
}


ErrorCode MemoryBlock::set(const void* const ptrNewData, const U32 newSize)
{
    ErrorCode err = (newSize > size) ? ER_DATA_TOO_LARGE : ER_SUCCESS;

    if (err == ER_SUCCESS)
    {
        size = newSize;       // update the data size
        memcpy(ptrData, ptrNewData, newSize);
    }
      
    return (err);     
}


U32 MemoryBlock::getSize(void) const
{
    return (size);
}


void MemoryBlock::resize(const U32 newDataSize)
{
    // ensure that new size is equal to or less than the originally allocated data size
    size = ((newDataSize <= maxSize) && (newDataSize > 0)) ? newDataSize : size;
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
