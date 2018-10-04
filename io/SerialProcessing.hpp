#ifndef SERIAL_PROCESSING_HPP
#define SERIAL_PROCESSING_HPP

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include "Singleton.hpp"
#include "SerialComm.hpp"
#include "Task.hpp"

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

class SerialProcessing : public Task
{
    public:

        explicit SerialProcessing();

        void init(void);
        void exec(void);

    protected:

    private:

        // Unused and disabled.
        ~SerialProcessing() {}
        SerialProcessing(const SerialProcessing&);
        SerialProcessing& operator=(const SerialProcessing&);
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------
#define serialProc Singleton<SerialProcessing>::getInstance()

#endif /* SERIAL_PROCESSING_HPP */
