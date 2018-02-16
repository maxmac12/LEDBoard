//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "StatCommand.hpp"
#include "TaskScheduler.hpp"
#include "MaintenanceComm.hpp"
#include <string.h>

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

StatCommand::StatCommand()
{
    // Do nothing.
}


ErrorCode StatCommand::exec(const CStr subject, const CStr value1, const CStr value2, const CStr value3)
{
    bool reset = (strncmp(subject, "reset", 6UL) == 0);

    for (U32 taskIdx = 0; taskIdx < taskScheduler->getNumTasks(); taskIdx++)
    {
        if (taskScheduler->tasks[taskIdx] != NULL)
        {
            if (reset)
            {
                taskScheduler->tasks[taskIdx]->resetMaxExecTime();
            }

            maintComm->sendData(MaintenanceComm::MAX_CONSOLE_LINE_LEN,
                     "Task[%d] \"%s\" last=%dms max=%dms exceeds=%d faulted=%d\r\n",
                        taskIdx,
                        taskScheduler->tasks[taskIdx]->getName(),
                        taskScheduler->tasks[taskIdx]->getLastExecTime(),
                        taskScheduler->tasks[taskIdx]->getMaxExecTime(),
                        taskScheduler->tasks[taskIdx]->getNumTimeBudgetExceeds(),
                        static_cast<S32>(taskScheduler->tasks[taskIdx]->isFaulted()));
        }
    }

    return (ER_SUCCESS);
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
