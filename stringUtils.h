#ifndef STRING_H
#define STRING_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**  Included Files 
*****************************************************************************/
#include "dataTypes.h"

/*****************************************************************************
**  Public Function Prototypes 
*****************************************************************************/

S8* strnpbrk(const S8* ptrStr, const S8* ptrToken, U32 maxChars, U32 tokenSize);
S8* strnsep(S8** pptrStr, const S8* ptrToken, U32 maxChars, U32 tokenSize);

#ifdef __cplusplus
}
#endif

#endif
