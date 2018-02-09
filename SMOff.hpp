#ifndef SMOFF_HP
#define SMOFF_HP

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

class SMOff : public StateMachine
{
    public:

        explicit SMOff();

        void init(void);
        void run(void);
        void reset(void);

    protected:

    private:

        enum States
        {
            IDLE_STATE,
            OFF_STATE,
            NUM_STATES
        };

        void idle(void);
        void turnLedsOff(void);

        // Define a function pointer type.
        typedef void (SMOff::*PtrStateFunc)(void);

        // Pointer to the current state.
        PtrStateFunc ptrCurrentState;

        // An array of pointers to the state functions.
        PtrStateFunc ptrStateFunc[NUM_STATES];

        // Unused and disabled.
        ~SMOff() {}
        SMOff(const SMOff&);
        SMOff& operator=(const SMOff&);
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#endif
