//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "Task.hpp"
#include "dataTypes.h"
#include "error.hpp"
#include "StopWatch.hpp"
#include <string.h>          // strncpy(), strncmp()

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

Task::Task(const Msec taskPeriod,
           const S8* ptrTaskName,
           const TaskIDEnum theTaskID) :
    period(taskPeriod),
    lastElapsedTime(0),
    maxExecTime(0),
    numExecTimeExceeds(0),
    ptrStopWatch(new StopWatch()),
    taskID(theTaskID),
    initIsFinished(false),
    taskIsFaulted(false)
{
    strncpy(name, ptrTaskName, MAX_NAME_LEN);
}


const S8* const Task::getName(void) const
{
    return (name);
}


Msec Task::getPeriod(void) const
{
    return (period);
}


Msec Task::getMaxExecTime(void) const
{
    return (maxExecTime);
}


void Task::resetMaxExecTime(void)
{
    maxExecTime = 0UL;
}


Msec Task::getLastExecTime(void) const
{
    return (lastElapsedTime);
}


U32 Task::getNumTimeBudgetExceeds(void) const
{
    return (numExecTimeExceeds);
}


bool Task::isInitialized(void) const
{
    return (initIsFinished);
}


bool Task::isFaulted(void) const
{
    return (taskIsFaulted);
}

//############################################################################
//  Protected Methods
//############################################################################

void Task::disableTask(const ErrorCode errCode)
{
    taskIsFaulted = true;
}

//############################################################################
//  Private Methods
//############################################################################
