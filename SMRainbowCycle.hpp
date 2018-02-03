#ifndef SMRAINBOWCYCLE_H
#define SMRAINBOWCYCLE_H

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

class SMRainbowCycle : public StateMachine
{
    public:

        SMRainbowCycle();

        void init(void);
        void run(void);
        void reset(void);

    protected:

    private:

        enum States
        {
            IDLE_STATE,
            INITIAL_STATE,
            UPDATE_PIXELS,
            UPDATE_STRIPS,
            NUM_STATES
        };

        void idle(void);
        void initialState(void);
        void updatePixels(void);
        void updateStrips(void);

        // Define a function pointer type.
        typedef void (SMRainbowCycle::*PtrStateFunc)(void);

        // Pointer to the current state.
        PtrStateFunc ptrCurrentState;

        // An array of pointers to the state functions.
        PtrStateFunc ptrStateFunc[NUM_STATES];

        U32 currentStrip;
        U32 wheelPosition;

        // Unused and disabled.
        ~SMRainbowCycle() {}
        SMRainbowCycle(const SMRainbowCycle&);
        SMRainbowCycle& operator=(const SMRainbowCycle&);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
