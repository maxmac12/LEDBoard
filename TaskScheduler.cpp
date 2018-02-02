//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "ConsoleCtrl.hpp"
#include "MaintenanceComm.hpp"
#include "Queue.hpp"
#include "StopWatch.hpp"
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

StopWatch* ledTimer = new StopWatch();

static void blinkLED(void);

//############################################################################
//  Public Methods
//############################################################################

// TODO: Create basic task scheduler.
void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);     // Configure LED pin
    digitalWrite(LED_BUILTIN, HIGH);  // Turn LED ON
    ledTimer->start(LED_PERIOD);      // Initialize the LED timer

    consoleCtrl->init();
    maintComm->init();
}


void loop(void)
{
    blinkLED();
    maintComm->exec();
    consoleCtrl->exec();
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################

static void blinkLED(void)
{
    static uint8_t LED_STATE = HIGH;

    if (ledTimer->timerHasExpired())
    {
        // Flash on board LED every second
        digitalWrite(13, LED_STATE);
        LED_STATE = (HIGH == LED_STATE) ? LOW : HIGH;
        ledTimer->start(LED_PERIOD);  // Reset timer
    }
}
