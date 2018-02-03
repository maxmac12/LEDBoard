#ifndef SMWHITEOVERRAINBOW_H
#define SMWHITEOVERRAINBOW_H

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

class SMWhiteOverRainbow : public StateMachine
{
    public:

        explicit SMWhiteOverRainbow();

        // State machine functions.
        void init(void);
        void run(void);
        void reset(void);

        // Interface functions.
        void setWhiteLength(U32 length);
        void setWhiteSpeed(Msec speed);

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
        typedef void (SMWhiteOverRainbow::*PtrStateFunc)(void);

        // Pointer to the current state.
        PtrStateFunc ptrCurrentState;

        // An array of pointers to the state functions.
        PtrStateFunc ptrStateFunc[NUM_STATES];

        U32 currentStrip;
        U32 wheelPosition;
        Msec whiteSpeed;
        U8 whiteLength;
        U8 head;
        U8 tail;

        // Unused and disabled.
        ~SMWhiteOverRainbow() {}
        SMWhiteOverRainbow(const SMWhiteOverRainbow&);
        SMWhiteOverRainbow& operator=(const SMWhiteOverRainbow&);
};

//--------------------------------------------------------------------
//  Global Definitions
//--------------------------------------------------------------------

#endif
