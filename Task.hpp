#ifndef TASK_H
#define TASK_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "error.hpp"
#include "StopWatch.hpp"
#include "appconfig.hpp"
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

class Task
{
    public:

        explicit Task(const Msec taskPeriod,
                      const S8* ptrTaskName,
                      const TaskIDEnum theTaskID);

        virtual void init(void) = 0;
        virtual void exec(void) = 0;

        const S8* const getName(void) const;
        Msec getPeriod(void) const;
        Msec getMaxExecTime(void) const;
        void resetMaxExecTime(void);
        Msec getLastExecTime(void) const;
        U32 getNumTimeBudgetExceeds(void) const;
        bool isInitialized(void) const;
        bool isFaulted(void) const;

        // Unused and disabled.
        virtual ~Task() {}

    protected:

        void disableTask(const ErrorCode errCode = ER_FAIL);

        Msec period;              // Period of task execution (ms).
        Msec lastElapsedTime;     // Elapsed time of last execution.
        Msec maxExecTime;         // Longest execution time of task.
        U32  numExecTimeExceeds;  // Number of times the time budget was exceeded.

        StopWatch* ptrStopWatch;  // Stop watch to keep track of time periods.

        TaskIDEnum taskID;        // the ID of the task.

    private:

        S8 name[MAX_NAME_LEN];    // Name of the task.

        bool initIsFinished;      // Task has passed init() stage
        bool taskIsFaulted;       // Keep track of fault status of task.

        // Unused and disabled.
        Task(const Task&);
        Task& operator=(const Task&);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
