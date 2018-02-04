#ifndef SMCOLOR_H
#define SMCOLOR_H

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

class SMColor : public StateMachine
{
    public:

        explicit SMColor();

        void init(void);
        void run(void);
        void reset(void);

    protected:

    private:

        enum States
        {
            IDLE_STATE,
            SET_COLOR_STATE,
            NUM_STATES
        };

        void idle(void);
        void setColor(void);

        // Define a function pointer type.
        typedef void (SMColor::*PtrStateFunc)(void);

        // Pointer to the current state.
        PtrStateFunc ptrCurrentState;

        // An array of pointers to the state functions.
        PtrStateFunc ptrStateFunc[NUM_STATES];

        U32 currentColor;
        bool runUntilColorChange;  // Allows the state machine to run the first execution of run() even if the color hasn't changed.

        // Unused and disabled.
        ~SMColor() {}
        SMColor(const SMColor&);
        SMColor& operator=(const SMColor&);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
