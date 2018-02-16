#ifndef COMMONDATA_H
#define COMMONDATA_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "dataTypes.h"
#include "error.hpp"
#include "StopWatch.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------
 
//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

template <class T>
class CommonData
{
    public:

        // Maximum length of data variable name.
        static const U32 MAX_NAME_LEN = 32;

        // Maximum length of data bytes
        static const U32 MAX_DATA_LEN = 32;

        explicit CommonData(const CStr dataName,
                            const T init_val,
                            const bool init_valid = false,
                            const Flags init_flags = 0x0,
                            const Msec init_stale_time = 0);


        const S8* getName(void) const;
        void set(const T val, const bool crowbar = false);
        void set(const U8* const ptrNewData, const U32 newDataSize = 0, const bool crowbar = false);
        void unlock(void);
        const T get(void) const;
        void setFlags(const Flags flagsToSet, const bool flagSetting);
        Flags getFlags(void) const;
        bool getFlag(const U32 bit) const;
        void setValid(const bool new_valid);
        bool isValid(void) const;
        bool isStale(void) const;
        Msec getStaleTimeout(void) const;
        void resetStaleTimer(void);
        void enableStaleTimer(const Msec staleTime);
        bool isLocked(void) const;

    protected:

    private:

        S8 name[MAX_NAME_LEN];      // Name of data.    
        T data;                     // The data.    
        Flags flags;                // Data flags.                
        bool locked;                // A lock to prevent changes from set().
        bool valid;                 // Validity

        Msec msUntilStale;          // Milliseconds of non-activity before data
                                    // declared stale.
        StopWatch stopWatch;        // Stop watch to keep track of time periods.

        // Unused and disabled.
        ~CommonData() {}
        CommonData(const CommonData&);
        CommonData& operator=(const CommonData&);
};


//############################################################################
//  Public Methods
//############################################################################

template <class T>
void CommonData<T>::unlock(void)
{
    locked = false;
}


template <class T>
const T CommonData<T>::get(void) const
{
    return (data);
}


template <class T>
const S8* CommonData<T>::getName(void) const
{
    return (name);
}


template <class T>
void CommonData<T>::setFlags(const Flags flagsToSet, const bool flagSetting)
{
    flags = flagSetting ? (flags | flagsToSet) : (flags & ~flagsToSet);

    // restart timer
    stopWatch.start(msUntilStale);
}


template <class T>
Flags CommonData<T>::getFlags(void) const
{
    return (flags);
}


template <class T>
bool CommonData<T>::getFlag(const U32 bit) const
{
    return (bit < 32UL ? (flags & (1UL << bit)) == (1UL << bit) : false);
}


template <class T>
void CommonData<T>::setValid(const bool new_valid)
{
    valid = new_valid;
}


template <class T>
bool CommonData<T>::isValid(void) const
{
    return (valid);
}


template <class T>
bool CommonData<T>::isStale(void) const
{
    // if stale timeout value is 0, then stale calculation is disabled
    return (((msUntilStale != static_cast<Msec>(0)) && !locked) ? stopWatch.timerHasExpired() : false);
}


template <class T>
Msec CommonData<T>::getStaleTimeout(void) const
{
    return (msUntilStale);
}


template <class T>
void CommonData<T>::resetStaleTimer(void)
{
    stopWatch.start(msUntilStale);
}


template <class T>
void CommonData<T>::enableStaleTimer(const Msec staleTime)
{
    msUntilStale = staleTime;
    resetStaleTimer();
}


template <class T>
bool CommonData<T>::isLocked(void) const
{
    return (locked);
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#endif
