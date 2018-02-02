//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "CommonData.hpp"
#include "stdlib.h"         // memcpy()
#include "string.h"         // strncpy()
#include "MemoryBlock.hpp"

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

template <>
CommonData<S32>::CommonData(const CStr dataName,
                            const S32 init_val,
                            const bool init_valid,
                            const Flags init_flags,
                            const Msec init_stale_time) :
    data(init_val),
    flags(init_flags), 
    locked(false),
    valid(init_valid),
    msUntilStale(init_stale_time)
{
    strncpy(name, dataName, CommonData<S32>::MAX_NAME_LEN);
}


template <>
CommonData<S64>::CommonData(const CStr dataName,
                            const S64 init_val,
                            const bool init_valid,
                            const Flags init_flags,
                            const Msec init_stale_time) :
    data(init_val),
    flags(init_flags),
    locked(false),
    valid(init_valid),
    msUntilStale(init_stale_time)
{
    strncpy(name, dataName, CommonData<S64>::MAX_NAME_LEN);
}


template <>
CommonData<MemPtr>::CommonData(const CStr dataName,
                                     const MemPtr init_val,
                                     const bool init_valid,
                                     const Flags init_flags,
                                     const Msec init_stale_time) :
    data(init_val),
    flags(init_flags),
    locked(false),
    valid(init_valid),
    msUntilStale(init_stale_time)
{
    strncpy(name, dataName, CommonData<MemPtr>::MAX_NAME_LEN);
}


template <>
CommonData<bool>::CommonData(const CStr dataName,
                             const bool init_val,
                             const bool init_valid,
                             const Flags init_flags,
                             const Msec init_stale_time) :
    data(init_val),
    flags(init_flags), 
    locked(false),
    valid(init_valid),
    msUntilStale(init_stale_time)
{
    strncpy(name, dataName, CommonData<bool>::MAX_NAME_LEN);
}


template <>
void CommonData<S32>::set(const S32 val, const bool crowbar)
{
    locked = crowbar ? false : locked;  // release lock if crowbarring

    if (!locked)
    {
        data = val;
        stopWatch.start(msUntilStale);
    }

    locked = crowbar ? true : locked;   // engage lock if crowbarring
}


template <>
void CommonData<S64>::set(const S64 val, const bool crowbar)
{
    locked = crowbar ? false : locked;  // release lock if crowbarring

    if (!locked)
    {
        data = val;
        stopWatch.start(msUntilStale);
    }

    locked = crowbar ? true : locked;   // engage lock if crowbarring
}


template <>
void CommonData<MemPtr>::set(const MemPtr ptrVal, const bool crowbar)
{
    locked = crowbar ? false : locked;  // release lock if crowbarring

    if (!locked && (ptrVal != NULL))
    {
        data = ptrVal;
        stopWatch.start(msUntilStale);
    }

    locked = crowbar ? true : locked;   // engage lock if crowbarring
}


template<>
void CommonData<MemPtr>::set(const U8* const ptrNewData, const U32 newDataSize, const bool crowbar)
{
    locked = crowbar ? false : locked;  // release lock if crowbarring

    if (!locked && (data != NULL))
    {
        // resize the memory block (new size valid up to originally allocated size)
        data->resize(newDataSize);

        memcpy(data->get(), ptrNewData, data->getSize());

        stopWatch.start(msUntilStale);
    }

    locked = crowbar ? true : locked;   // engage lock if crowbarring
}


template <>
void CommonData<bool>::set(const bool val, const bool crowbar)
{
    locked = crowbar ? false : locked;  // release lock if crowbarring

    if (!locked)
    {
        data = val;
        stopWatch.start(msUntilStale);
    }

    locked = crowbar ? true : locked;   // engage lock if crowbarring
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
