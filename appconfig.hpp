#ifndef APPCONFIG_H
#define APPCONFIG_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "dataTypes.h"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

const U32 MAX_NAME_LEN = 32;   // Maximum length of task name.

//#############################################################################
// Task execution period (in milliseconds)
//#############################################################################

const Msec TASK_CONSOLE_PERIOD       = 20;
const Msec TASK_MAINT_PERIOD         = 20;
const Msec TASK_LED_MODE_CTRL_PERIOD = 20;
const Msec CYCLIC_ENGINE_PERIOD      = 20;

//#############################################################################
// Task names
//#############################################################################

const S8 TASK_MAINT_NAME[MAX_NAME_LEN]          = "Maintenance Comm Task";
const S8 TASK_CONSOLE_NAME[MAX_NAME_LEN]        = "Console Interface Task";
const S8 TASK_LED_MODE_CTRL_NAME[MAX_NAME_LEN]  = "LED Mode Control Task";
const S8 CYCLIC_ENGINE_NAME[MAX_NAME_LEN]       = "Cyclic Task Engine";

enum QueueIDEnum
{
    QID_MAINT_RX = 0,
    QID_MAINT_TX,

    QID_NONE
};

enum TaskIDEnum
{
    TID_TASK_SCHEDULER = 0,
    TID_CONSOLE_CTRL,
    TID_MAINT_COMM,
    TID_LED_MODE_CTRL,

    NUM_TASKS
};

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#endif
