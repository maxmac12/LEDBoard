#ifndef LEDMODECONTROL_H
#define LEDMODECONTROL_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "LEDControl.hpp"
#include "Task.hpp"
#include "Singleton.hpp"
#include "StateMachine.hpp"
#include "StopWatch.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class LedModeControl : public Task
{
    public:

        explicit LedModeControl();

        void init(void);
        void exec(void);

        // Unused and disabled.
        ~LedModeControl() {}
        LedModeControl(const LedModeControl&);
        LedModeControl& operator=(const LedModeControl&);

    protected:

    private:

        void readAnalogBrightness(void);
        void readMomentarySwitch(void);

        StateMachine* ptrLedStateMachines[NUM_LED_MODES];
        StopWatch* ptrBrightnessTimer;
        StopWatch* ptrMomSwitchDebounce;
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#define ledModeCtrl Singleton<LedModeControl>::getInstance()

#endif
