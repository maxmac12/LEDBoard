//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "TaskScheduler.hpp"
#include "dataTypes.h"
#include <core_pins.h>
#include <pins_arduino.h>

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

/////////////////////////////////////////////////////////
// ADD TASKS HERE. Unused task slots must be NULL.
/////////////////////////////////////////////////////////
//tasks[] =
//{
//    ledModeCtrl,
//    maintComm,
//    consoleCtrl,
//
//    NULL
//};


//############################################################################
//  Public Methods
//############################################################################

TaskScheduler::TaskScheduler() :
    numTasks(0),
    ledTimer(new StopWatch())
{
    // Do nothing.
}


void TaskScheduler::init(void)
{
    // Configure the LED heart beat.
    pinMode(LED_BUILTIN, OUTPUT);     // Configure LED pin
    digitalWrite(LED_BUILTIN, HIGH);  // Turn LED ON
    ledTimer->start(LED_PERIOD);      // Initialize the LED timer
}


void TaskScheduler::exec(void)
{
    // Execute all tasks.
    for (U32 i = 0; i < numTasks; i++)
    {
        if (tasks[i] != NULL)
        {
            if (!tasks[i]->isFaulted())
            {
                // Execute the current task.
                tasks[i]->pre();
                tasks[i]->exec();
                tasks[i]->post();
            }
        }
    }

    // Blink the on board LED to indicate the software is still running.
    heartBeatLED();
}


ErrorCode TaskScheduler::addTask(Task* ptrTask)
{
    ErrorCode err = (numTasks < MAX_TASKS) ? ER_SUCCESS : ER_SYS_TOO_MANY_TASKS;

    if (err == ER_SUCCESS)
    {
        // Add the task and then initialize it.
        tasks[numTasks] = ptrTask;
        tasks[numTasks]->init();
        numTasks++;
    }

    return (err);
}


U32 TaskScheduler::getNumTasks(void)
{
    return numTasks;
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

void TaskScheduler::heartBeatLED(void)
{
    static U8 LED_STATE = HIGH;

    if (ledTimer->timerHasExpired())
    {
        // Flash on board LED every second
        digitalWrite(13, LED_STATE);
        LED_STATE = (HIGH == LED_STATE) ? LOW : HIGH;
        ledTimer->start(LED_PERIOD);  // Reset timer
    }
}
