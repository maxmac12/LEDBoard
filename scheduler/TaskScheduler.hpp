#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "dataTypes.h"
#include "Singleton.hpp"
#include "Task.hpp"
#include "StopWatch.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

#define HEARTBEAT_LEAD_PERIOD  1000  // 1 second

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class TaskScheduler
{
    friend class StatCommand;  // For access to task information.

    public:

        explicit TaskScheduler();

        void init(void);
        void exec(void);
        ErrorCode addTask(Task* const ptrTask);
        U32 getNumTasks(void);

    protected:

    private:

        // Maximum number of tasks in one cyclic execution frame.
        static const U32 MAX_TASKS = 32;

        void heartBeatLED(void);

        Task* tasks[MAX_TASKS];

        U32 numTasks;
        StopWatch* ledTimer;

        // Unused and disabled.
        ~TaskScheduler() {}
        TaskScheduler(const TaskScheduler&);
        TaskScheduler& operator=(const TaskScheduler&);
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------
#define taskScheduler Singleton<TaskScheduler>::getInstance()

#endif
