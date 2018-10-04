#ifndef SMSPECTRUM_HPP
#define SMSPECTRUM_HPP

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

class SMSpectrum : public StateMachine
{
    public:

        explicit SMSpectrum();

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
        typedef void (SMSpectrum::*PtrStateFunc)(void);

        // Pointer to the current state.
        PtrStateFunc ptrCurrentState;

        // An array of pointers to the state functions.
        PtrStateFunc ptrStateFunc[NUM_STATES];

        U32 currentStrip;

        // Unused and disabled.
        ~SMSpectrum() {}
        SMSpectrum(const SMSpectrum&);
        SMSpectrum& operator=(const SMSpectrum&);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
