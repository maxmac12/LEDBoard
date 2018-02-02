#ifndef ERROR_H
#define ERROR_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Defines 
//----------------------------------------------------------------------------

// Error codes indicating success or failure of function calls 
enum ErrorCode
{
    ER_SUCCESS              = 0,   // Success
    ER_FAIL                 = 1,   // Failure
    ER_NULL_POINTER         = 2,   // A pointer is NULL when it should not be
    ER_VALUE_OUTSIDE_RANGE  = 3,   // A value is outside of the range it should be
    ER_SYS_INIT_TASK        = 4,   // Failed to initialize task
    ER_SYS_TOO_MANY_TASKS   = 5,   // Too many tasks
    ER_FRAME_INVALID        = 6,   // Task frame index invalid
    ER_QUEUE_FULL           = 7,   // Queue full
    ER_QUEUE_EMPTY          = 8,   // Queue empty
    ER_DATA_TOO_LARGE       = 9,   // Data too large to fit
    ER_HASH_DUPE            = 10,  // Item already exists in hash
    ER_HASH_HALF_FULL       = 11,  // Hash table more than half full
    ER_HASH_INSERT_FAIL     = 12,
    ER_VALUE_NOT_IN_HASH    = 13,  // Value not found in hash table
    ER_INVALID_TASK_PERIOD  = 14,  // Task period lower than or not divisible by CYCLIC_ENGINE_PERIOD
    EVT_EVENT_LEG_START     = 15,  // The start of the event log for this leg
    ER_HASH_TABLE_SETUP     = 16,  // Error with hash table setup - tried to address outside of table range.
    ER_TASK_OVERRUN         = 17,  // Task frame period has been exceeded.
    ER_QUEUE_NOT_INIT       = 18,  // Queue can't be initialized
    ER_CONSOLE_INIT         = 19,
    ER_NEW_FAULT            = 20
};

// Module codes for each part of our system
enum ModuleCode
{
    MC_NONE          = 0,
    MC_MAIN_CONTROL  = 1,
    MC_LED_CONTROL   = 2,
    MC_CONSOLE_APP   = 3,
    MC_MAINT_COMM    = 4
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
