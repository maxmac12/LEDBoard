//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "StopWatch.hpp"
#include "dataTypes.h"
#include "core_pins.h"    // millis()

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

StopWatch::StopWatch() :
    startTime(0),
    waitDuration(0)
{
    // do nothing
}


void StopWatch::start(const Msec timePeriod)
{
    startTime = millis();
    waitDuration = timePeriod;
}


Msec StopWatch::stop(void)
{
    waitDuration = 0UL;
    return (getDifference(startTime, millis()));
}


bool StopWatch::timerHasExpired(void) const
{
    return ((waitDuration == 0UL) || (getDifference(startTime, millis()) > waitDuration));
}


Msec StopWatch::getDifference(const Msec beginTime, const Msec endTime) const
{
    // account for potential rollover before returning
    return ((beginTime > endTime) ? (endTime + (MAX_UINT - beginTime) + 1UL) : (endTime - beginTime));
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
