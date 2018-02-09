//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "ConsoleCtrl.hpp"
#include "LEDControl.hpp"
#include "LEDModeControl.hpp"
#include "MaintenanceComm.hpp"
#include "Task.hpp"
#include "StopWatch.hpp"
#include "appconfig.hpp"
#include "dataTypes.h"
#include <core_pins.h>
#include <pins_arduino.h>
#include <wiring.h>

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------
#define LED_PERIOD  1000  // 1 second

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

// Add additional tasks here.
Task* tasks[] =
{
    ledModeCtrl,
    maintComm,
    consoleCtrl,

    NULL          // end of list - DO NOT REMOVE!!!
};

StopWatch* ledTimer  = new StopWatch();  // LED Blink timer.

static void blinkLED(void);

//############################################################################
//  Public Methods
//############################################################################

void setup(void)
{
    ledCtrl->init();

    pinMode(LED_BUILTIN, OUTPUT);     // Configure LED pin
    digitalWrite(LED_BUILTIN, HIGH);  // Turn LED ON
    ledTimer->start(LED_PERIOD);      // Initialize the LED timer

    // Initialize all tasks.
    for (U32 i = 0; tasks[i] != NULL; i++)
    {
        tasks[i]->init();
    }
}


void loop(void)
{
    blinkLED();  // Blink on board LED

    // Process all tasks.
    for (U32 i = 0; tasks[i] != NULL; i++)
    {
        // Execute tasks that are ready to run. Skip faulted tasks.
        // TODO: Check if task period has elapsed before executing task.
        if (!tasks[i]->isFaulted())
        {
            // Execute the current task.
            tasks[i]->exec();
        }
    }
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

static void blinkLED(void)
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
