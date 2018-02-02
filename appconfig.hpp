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
const Msec CYCLIC_ENGINE_PERIOD      = 20;

//#############################################################################
// Task names
//#############################################################################

const S8 TASK_MAINT_NAME[MAX_NAME_LEN]    = "Maintenance Comm Task";
const S8 TASK_CONSOLE_NAME[MAX_NAME_LEN]  = "Console Interface Task";
const S8 CYCLIC_ENGINE_NAME[MAX_NAME_LEN] = "Cyclic Task Engine";

enum QueueIDEnum
{
    QID_MAINT_RX  = 1,
    QID_MAINT_TX  = 2,

    QID_NONE      = 3
};

enum TaskIDEnum
{
    TID_CONSOLE_CTRL    = 1,
    TID_MAINT_COMM      = 2,
    TID_TASK_SCHEDULER  = 3
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
