//----------------------------------------------------------------------------
//  Included Files 
//----------------------------------------------------------------------------
#include "HashTable.hpp"
#include "dataTypes.h"
#include "MemoryBlock.hpp"

//----------------------------------------------------------------------------
//  Local Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Public Data 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Private Data 
//----------------------------------------------------------------------------

//############################################################################
//  Public Methods
//############################################################################

template <>
HashTable<S32>::HashTable(const U32 tableSize) :
    pptrHashEntries(NULL),
    size(nextPrime(tableSize)), // round up to next prime number for hash function
    numSlotsUsed(0)   
{
    pptrHashEntries = new HashEntry*[size];
    size = (pptrHashEntries == NULL) ? 0UL : size;

    for (U32 entry = 0; entry < size; entry++)
    {
        pptrHashEntries[entry] = NULL;
    }
}


template <>
HashTable<S64>::HashTable(const U32 tableSize) :
    pptrHashEntries(NULL),
    size(nextPrime(tableSize)), // round up to next prime number for hash function
    numSlotsUsed(0)
{
    pptrHashEntries = new HashEntry*[size];
    size = (pptrHashEntries == NULL) ? 0UL : size;

    for (U32 entry = 0; entry < size; entry++)
    {
        pptrHashEntries[entry] = NULL;
    }
}


template <>
HashTable<MemPtr>::HashTable(const U32 tableSize) :
    pptrHashEntries(NULL),
    size(nextPrime(tableSize)),  // round up to next prime number for hash function
    numSlotsUsed(0)   
{
    pptrHashEntries = new HashEntry*[size];
    size = (pptrHashEntries == NULL) ? 0UL : size;

    for (U32 entry = 0; entry < size; entry++)
    {
        pptrHashEntries[entry] = NULL;
    }
}


template <>
HashTable<bool>::HashTable(const U32 tableSize) :
    pptrHashEntries(NULL),
    size(nextPrime(tableSize)),  // round up to next prime number for hash function
    numSlotsUsed(0)   
{
    pptrHashEntries = new HashEntry*[size];
    size = (pptrHashEntries == NULL) ? 0UL : size;

    for (U32 entry = 0; entry < size; entry++)
    {
        pptrHashEntries[entry] = NULL;
    }
}

//############################################################################
//  Protected Methods
//############################################################################

//############################################################################
//  Private Methods
//############################################################################
