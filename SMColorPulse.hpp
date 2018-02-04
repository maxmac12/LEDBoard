#ifndef SMCOLORPULSE_H
#define SMCOLORPULSE_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "StateMachine.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class SMColorPulse : public StateMachine
{
    public:

        explicit SMColorPulse();

        void init(void);
        void run(void);
        void reset(void);
        void setPulseSpeed(Msec speed);

    protected:

    private:

        enum States
        {
            IDLE_STATE,
            INITIAL_STATE,
            FADE_IN_STATE,
            FADE_OUT_STATE,
            DELAY_STATE,
            NUM_STATES
        };

        void idle(void);
        void initialState(void);
        void fadeIn(void);
        void fadeOut(void);
        void delay(void);

        // Define a function pointer type.
        typedef void (SMColorPulse::*PtrStateFunc)(void);

        // Pointer to the current state.
        PtrStateFunc ptrCurrentState;

        // An array of pointers to the state functions.
        PtrStateFunc ptrStateFunc[NUM_STATES];

        U8 brightness;
        Msec pulseSpeed;
        bool fadeInFlag;

        // Unused and disabled.
        ~SMColorPulse() {}
        SMColorPulse(const SMColorPulse&);
        SMColorPulse& operator=(const SMColorPulse&);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
