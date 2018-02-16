#ifndef SINGLETON_H
#define SINGLETON_H

//----------------------------------------------------------------------------
//  Included Files
//----------------------------------------------------------------------------
#include <stddef.h>

//----------------------------------------------------------------------------
//  Public Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Function Prototypes
//----------------------------------------------------------------------------

template <class T>
class Singleton
{
    public:

        static T* getInstance(void)
        {
            if (ptrInstance == NULL)
            {
                // check pointer again in case it got updated after the first check
                ptrInstance = ((ptrInstance == NULL) ? new T() : ptrInstance);
            }

            return (ptrInstance);
        }

    private:

        static T* ptrInstance;  // Pointer to static data.

        // Unused and disabled.
        Singleton() {}
        ~Singleton() {}
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);
};

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

// static variable initialization
template <class T> T* Singleton<T>::ptrInstance = NULL;

#endif
