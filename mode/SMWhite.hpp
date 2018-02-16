#ifndef SMWHITE_H
#define SMWHITE_H

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

class SMWhite : public StateMachine
{
    public:

        explicit SMWhite();

        void init(void);
        void run(void);
        void reset(void);

    protected:

    private:

        enum States
        {
            IDLE_STATE,
            WHITE_STATE,
            NUM_STATES
        };

        void idle(void);
        void setWhiteLight(void);

        // Define a function pointer type.
        typedef void (SMWhite::*PtrStateFunc)(void);

        // Pointer to the current state.
        PtrStateFunc ptrCurrentState;

        // An array of pointers to the state functions.
        PtrStateFunc ptrStateFunc[NUM_STATES];

        // Unused and disabled.
        ~SMWhite() {}
        SMWhite(const SMWhite&);
        SMWhite& operator=(const SMWhite&);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
