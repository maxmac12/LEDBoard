#ifndef STOPWATCH_H
#define STOPWATCH_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "dataTypes.h"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------
 
//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class StopWatch
{
    public:

        explicit StopWatch();

        void start(const Msec timePeriod);
        Msec stop(void);
        bool timerHasExpired(void) const;
        Msec getDifference(const Msec beginTime, const Msec endTime) const;

        // Unused and disabled.
        ~StopWatch() {};

    protected:

    private:
        
        Msec startTime;     // Keep track of start time.
        Msec waitDuration;  // Keep track of time to wait.

        // Unused and disabled.
        StopWatch(const StopWatch&);
        StopWatch& operator=(const StopWatch&);
};

//----------------------------------------------------------------------------
//  Global Definitions 
//----------------------------------------------------------------------------

#endif
