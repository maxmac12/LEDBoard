/**********************************************************************
**  Included Files
**********************************************************************/
#include "stringUtils.h"
#include "dataTypes.h"

/**********************************************************************
**  Local Defines
**********************************************************************/

/**********************************************************************
**  Public Data
**********************************************************************/

/**********************************************************************
**  Private Data
**********************************************************************/

/**********************************************************************
**  Private Function Prototypes
**********************************************************************/

/*#####################################################################
**  Public Functions
#####################################################################*/

/****************************************************************************
**  Function Name: strnpbrk()
**
**  Purpose:
**      Finds first occurrence of any part of a token in a set of characters.
**
**  Parameters:
**      [in] ptrStr
**          A string.
**      [in] ptrToken
**          A list of token characters to search for in the input string.
**      [in] maxChars
**          Maximum number of characters to search for
**      [in] tokenSize
**          Number of token characters.
**
**  Global Data Used or Changed:
**      None.
**
**  Return Value: S8*
**      Pointer to the first character in the part of the input string that 
**      matches any characters in a token string.
**
**  Notes:
**      Example:
**      S8* str1 = "one two three";
**      S8* token = "e";
**      S8* first_chr = strnpbrk(str1, token, 14);  // returns pointer to 3rd character of str1
**
**      This function is not thread-safe.
**
**      This function limits the number of characters to search for by a user
**      input parameter, so any character matches must occur prior to the limit
**      of characters imposed by the caller of the function.
**
****************************************************************************/
S8* strnpbrk(const S8* ptrStr, const S8* ptrToken, U32 maxChars, U32 tokenSize)
{
    S8* ptrCurr, *ptrCurrToken;
    S8* ptrRet = NULL;
    const U32 TOKEN_SIZE = tokenSize;

    for (ptrCurr = (S8*)ptrStr; (ptrCurr != NULL) && (maxChars-- > 0); ++ptrCurr)
    {
        if (*ptrCurr == '\0')
        {
            break;
        }

        tokenSize = TOKEN_SIZE;
        for (ptrCurrToken = (S8*)ptrToken; (ptrCurrToken != NULL) && (tokenSize-- > 0); ++ptrCurrToken)
        {
            if (*ptrCurrToken == '\0')
            {
                break;
            }

            if (*ptrCurr == *ptrCurrToken)
            {
                ptrRet = ptrCurr;
                break;
            }
        }

        if (ptrRet != NULL)
        {
            break;
        }
    }

    return (ptrRet);
}


/****************************************************************************
**  Function Name: strnsep()
**
**  Purpose:
**      Separates string by the given token
**
**  Parameters:
**      [in, out] pptrStr
**          Pointer to a string; updated to the first character after the 
**          token
**      [in] ptrToken
**          A token to use for separating the input string
**      [in] maxChars
**          Maximum number of characters to process.
**      [in] tokenSize
**          Number of token characters.
**
**  Global Data Used or Changed:
**      None
**
**  Return Value:
**      Pointer to the first character delimited by the token
**
**  Notes:
**      Example:
**          char buf[16] = "one two three";
**          char* test, pbuf = buf;
**          test = strnsep(&pbuf, " ", 14, 2);    // test = "one"; pbuf = "two three"
**          test = strnsep(&pbuf, " ", 14, 2);    // test = "two"; pbuf = "three"
**          test = strnsep(&pbuf, " ", 14, 2);    // test = "three"; pbuf = NULL
**          test = strnsep(&pbuf, " ", 14, 2);    // test = NULL; pbuf = NULL
**
**      This function is not thread-safe.
**
**      This function limits the number of characters to be processed based on
**      the input parameters by the caller of this function.
**
****************************************************************************/
S8* strnsep(S8** pptrStr, const S8* ptrToken, U32 maxChars, U32 tokenSize)
{
    S8* ptrBegin = NULL;
    S8* ptrEnd;

    if (pptrStr != NULL)
    {
        ptrBegin = *pptrStr;

        ptrEnd = strnpbrk(ptrBegin, ptrToken, maxChars, tokenSize);
        if (ptrEnd != NULL)
        {
            *ptrEnd++ = '\0';
        }
        *pptrStr = ptrEnd;
    }

    return (ptrBegin);
}

/*#####################################################################
**  Private Functions
#####################################################################*/
