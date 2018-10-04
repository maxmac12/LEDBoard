//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "LEDControl.hpp"
#include "appconfig.hpp"
#include "StateMachine.hpp"
#include "SMColor.hpp"
#include "SMColorPulse.hpp"
#include "SMOff.hpp"
#include "SMRainbowCycle.hpp"
#include "SMSpectrum.hpp"
#include "SMWhite.hpp"
#include "SMWhiteOverRainbow.hpp"
#include "MaintenanceComm.hpp"
#include <core_pins.h>
#include "LedModeControl.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------
#define MOM_SW_PIN       A0  // Momentary switch pin.
#define MOM_SW_DEBOUNCE  40  // Debounce time (Msec) for momentary switch.

#define BRIGHTNESS_PIN        A1  // Brightness potentiometer pin.
#define BRIGHTNESS_TOLERANCE  2   // +/- tolerance of change for brightness.

#define ANALOG_READ_DELAY  60  // TODO: Update to get a better ADC average (128Hz, 32Hz, ...)

//----------------------------------------------------------------------------
//  Public Data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data
//----------------------------------------------------------------------------

//############################################################################
//  Public Methods
//############################################################################

LedModeControl::LedModeControl() :
    Task(TASK_LED_MODE_CTRL_PERIOD,
         TASK_LED_MODE_CTRL_NAME,
         TID_LED_MODE_CTRL),
    ptrBrightnessTimer(new StopWatch()),
    ptrMomSwitchDebounce(new StopWatch())
{
    // Create each LED mode state machine.
    ptrLedStateMachines[OFF]                = new SMOff();
    ptrLedStateMachines[WHITE]              = new SMWhite();
    ptrLedStateMachines[COLOR]              = new SMColor();
    ptrLedStateMachines[COLOR_PULSE]        = new SMColorPulse();
    ptrLedStateMachines[RAINBOW_CYCLE]      = new SMRainbowCycle();
    ptrLedStateMachines[WHITE_OVER_RAINBOW] = new SMWhiteOverRainbow();
    ptrLedStateMachines[SPECTRUM]           = new SMSpectrum();

    ptrBrightnessTimer->start(ANALOG_READ_DELAY);
}


void LedModeControl::init(void)
{
    // Initialize all LED state machines.
    for (U32 i = 0; i < NUM_LED_MODES; i++)
    {
        ptrLedStateMachines[i]->init();
    }

    // Configure momentary switch pin.
    pinMode(MOM_SW_PIN, INPUT_PULLUP);
}


void LedModeControl::exec(void)
{
    static LEDModes currentMode = OFF;

    LEDModes newMode = ledCtrl->getLedMode();

    // Check if the LED mode has changed.
    if (currentMode != newMode)
    {
        // State changed. Reset the current active
        ptrLedStateMachines[currentMode]->reset();
        currentMode = newMode;
    }

    // Run the active LED mode state machine.
    ptrLedStateMachines[currentMode]->run();

    // Read brightness ADC input.
    readAnalogBrightness();

    // Read the momentary switch input.
    readMomentarySwitch();
}


//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################


void LedModeControl::readAnalogBrightness(void)
{
    // TODO: Implement an IIR filter for the ADC values. Currently just an average over 4 readings.
    // TODO: Scale the readings to the MAX_BRIGHTNESS.
    const U8 MAX_ADC_READS = 4;
    static U8 numReadings = 0;
    static U32 brightAdcTotal;

    if (ptrBrightnessTimer->timerHasExpired())
    {
        // ADC values are 10-bit (0 - 1023)
        // Brightness values are between 0 - 255.
        // Divide the ADC value by 4 to get a brightness value.
        brightAdcTotal += (U32)analogRead(BRIGHTNESS_PIN);

        if (++numReadings >= MAX_ADC_READS)
        {
            // Get the average of the readings by dividing by 4 and
            // scale readings down to 8-bit brightness value.
            brightAdcTotal >>= 4;

            // Color Pulse state machine controls the brightness when running.
            // Add tolerance to ADC average.
            U8 currentBrightness = ledCtrl->getBrightness();

            if ((ledCtrl->getLedMode() != COLOR_PULSE) &&
               ((brightAdcTotal >= (currentBrightness + BRIGHTNESS_TOLERANCE)) ||
                (brightAdcTotal <= (currentBrightness - BRIGHTNESS_TOLERANCE))))
            {
                // Update the LED brightness based on the ADC reading.
                ledCtrl->setBrightness(brightAdcTotal);
            }

            // Reset ADC readings.
            numReadings = 0;
            brightAdcTotal = 0;
        }

        ptrBrightnessTimer->start(ANALOG_READ_DELAY);
    }

}


void LedModeControl::readMomentarySwitch(void)
{
    static bool debounceInProgress = false;  // Flag indicating if the switch is being debounced.
    static U8 lastSwState          = HIGH;   // Momentary switch is GND-Active.

    U8 currSwState = digitalRead(MOM_SW_PIN);

    // Check if switch state has changed.
    if (currSwState != lastSwState)
    {
        // Switch state has changed. Begin debounce.
        ptrMomSwitchDebounce->start(MOM_SW_DEBOUNCE);
        debounceInProgress = true;
    }

    // Check if the switch has been debounced.
    if ((ptrMomSwitchDebounce->timerHasExpired()) &&
        (debounceInProgress))
    {
        // Switch debounce complete.
        debounceInProgress = false;

        // Switch state has been debounced.
        if (LOW == currSwState)
        {
            // Switch is active. Cycle through the different LED modes.
            if ((LEDModes)(ledCtrl->getLedMode() + 1) >= NUM_LED_MODES)
            {
                // Rollover to the beginning of the LED mode list.
                ledCtrl->setLedMode((LEDModes)(0));
            }
            else
            {
                // Increment to the next LED mode.
                ledCtrl->setLedMode((LEDModes)(ledCtrl->getLedMode() + 1));
            }
        }
    }

    lastSwState = currSwState;
}
