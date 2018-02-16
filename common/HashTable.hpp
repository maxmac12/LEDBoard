#ifndef HASHTABLE_H
#define HASHTABLE_H

//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "dataTypes.h"       // U32, U16, etc.
#include "error.hpp"         // error codes
#include "string.h"          // strncmp(), strnlen()
#include "CommonData.hpp"

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
class HashTable
{
    public:
        static const U32 MAX_KEY_LEN = 32;    // Maximum size of hash key.

        explicit HashTable(const U32 tableSize);

        ErrorCode insert(const CStr key, CommonData<T>* const ptrValue);
        ErrorCode find(const CStr key, CommonData<T>*& ptrValue) const;
        U32 getRemainingSlots(void) const;
        U32 getSize(void) const;

    protected:

        struct HashEntry
        {
            explicit HashEntry(const CStr newKey,
                               CommonData<T>* const ptrNewObj);
            
            CommonData<T>* ptrObj;              // Pointer to the CommonData value.
            S8 key[HashTable<T>::MAX_KEY_LEN];  // The associated key.
        
            // Unused and disabled.
            ~HashEntry() {}
            HashEntry(const HashEntry&);
            HashEntry& operator=(const HashEntry& rhs);
        };

        bool isActive(const U32 currPos) const;
        U32 findPos(const CStr key) const;
        U32 hash(const CStr key) const;

    private:

        U32 nextPrime(U32 currPrime) const;
        bool isPrime(const U32 num) const;
    
        HashEntry** pptrHashEntries;    // Array of hash entries.
        U32 size;                       // Size of hash table.
        U32 numSlotsUsed;               // Number of slots used in the hash table.
    
        // Unused and disabled.
        ~HashTable() {}
        HashTable(const HashTable&);
        HashTable& operator=(const HashTable&);
};

//############################################################################
//  Public Methods
//############################################################################

template <class T>
ErrorCode HashTable<T>::insert(const CStr key,
                               CommonData<T>* const ptrValue)
{
    ErrorCode err = ER_SUCCESS;

    U32 currPos = findPos(key);
    if (isActive(currPos))
    {
        err = ER_HASH_DUPE;
    }      
    else
    {
        if (currPos < size)
        {
            if (ptrValue != NULL)
            {
                pptrHashEntries[currPos] = new HashEntry(key, ptrValue);

                if (pptrHashEntries[currPos] != NULL)
                {
                    // check if the table is more than half full
                    if (++numSlotsUsed > (size / 2UL))
                    {
                        err = ER_HASH_HALF_FULL;
                    }
                }
                else
                {
                    err = ER_NEW_FAULT;
                }
            }
            else
            {
                err = ER_NEW_FAULT;
            }
        }
        else
        {
            err = ER_HASH_TABLE_SETUP;
        }
    }      

    return (err);
}


template <class T>
ErrorCode HashTable<T>::find(const CStr key,
                             CommonData<T>*& ptrValue) const
{
    ErrorCode err = ER_VALUE_NOT_IN_HASH;

    U32 currPos = findPos(key);
    if (isActive(currPos))
    {
        ptrValue = pptrHashEntries[currPos]->ptrObj;
        err = ER_SUCCESS;
    }

    return (err);
}


template <class T>
U32 HashTable<T>::getRemainingSlots(void) const
{
    return (size - numSlotsUsed);
}


template <class T>
U32 HashTable<T>::getSize(void) const
{
    return (size);
}

//############################################################################
//  Protected Methods
//############################################################################

template <class T>
U32 HashTable<T>::findPos(const CStr key) const
{
    U32 collisionIdx = 0UL;
    U32 currPos = 0UL;

    if (size > 0)
    {
        currPos = hash(key) % size;
        while (currPos < size)
        {
            if (pptrHashEntries[currPos] != NULL)
            {
                if (strncmp(pptrHashEntries[currPos]->key, key, MAX_KEY_LEN) == 0)
                {
                    break;      // found the slot
                }
            }
            else
            {
                break;
            }

            currPos += ((2UL * ++collisionIdx) - 1UL);
            if (currPos >= size)
            {
                currPos -= size;  // wrap around if index goes over the size
            }
        }
    }

    return ((currPos < size) ? currPos : MAX_UINT);
}


template <class T>
U32 HashTable<T>::hash(const CStr key) const
{   
    // hash function for string key
    U32 hashVal = 0;

    // run through the key string and shift and XOR a hash value out of it
    const S32 KEYSTRLEN = strnlen(key, MAX_KEY_LEN);
    for (S32 strIdx = 0; strIdx < KEYSTRLEN; strIdx++)
    {
        hashVal = (hashVal << 5) ^ key[strIdx] ^ hashVal;
    }

    return (hashVal);
}


template <class T>
bool HashTable<T>::isActive(const U32 currPos) const
{
    bool ret = false;

    if (currPos < size)
    {
        ret = (pptrHashEntries[currPos] != NULL);
    }

    return (ret);
}


template <class T>
HashTable<T>::HashEntry::HashEntry(const CStr newKey,
                                   CommonData<T>* const ptrNewObj) :
    ptrObj(ptrNewObj)
{
    strncpy(key, newKey, HashTable<T>::MAX_KEY_LEN);
}

//############################################################################
//  Private Methods
//############################################################################

template <class T>
U32 HashTable<T>::nextPrime(U32 currPrime) const
{
    // prime numbers are always odd numbers, with the exception of 2;
    // 2 is the lowest prime number, so is excluded from this function 
    // (which only promises to return the _next_ prime number)
    if (currPrime % 2UL == 0UL)
    {
        currPrime++;
    }    
    
    while (!isPrime(currPrime))
    {
        currPrime += 2UL;
    }
    
    return (currPrime);
}  


template <class T>
bool HashTable<T>::isPrime(const U32 num) const
{
    bool bPrime = true;

    if ((num != 2UL) && (num != 3UL))   // 2 and 3 are prime numbers
    {
        if ((num == 1UL) || ((num % 2UL) == 0UL))
        {
            // prime numbers are odd numbers, with the exception of 2 as the
            // lowest possible prime number
            bPrime = false;
        }
        else
        {
            // check for divisors
            for (U32 currNum = 3UL; (currNum * currNum) <= num; currNum += 2UL)
            {
                // there's a divisor other than itself - it's not a prime
                if ((num % currNum) == 0UL)
                {
                    bPrime = false;
                }
            }                              
        }                        
    }

    return (bPrime);
}

//----------------------------------------------------------------------------
//  Global Definitions
//----------------------------------------------------------------------------

#endif
