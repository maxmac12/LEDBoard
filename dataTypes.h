#ifndef DATATYPES_H
#define DATATYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**  Included Files 
*****************************************************************************/

/*****************************************************************************
**  Public Defines 
*****************************************************************************/
     
/* Basic storage types for C and C++ */
typedef unsigned char               U8;      /* Unsigned 8-bit storage */
typedef char                        S8;      /* Signed 8-bit storage */
typedef unsigned short              U16;     /* Unsigned 16-bit storage */
typedef short                       S16;     /* Signed 16-bit storage */
typedef unsigned int                U32;     /* Unsigned 32-bit storage */
typedef int                         S32;     /* Signed 32-bit storage */
typedef unsigned long long int      U64;     /* Unsigned 64-bit storage */
typedef long long int               S64;     /* Signed 64-bit storage */

typedef U8                          Byte;    /* Byte storage */
typedef U16                         Word;    /* Word storage */
typedef U32                         DWord;   /* Double-word storage */
typedef U64                         QWord;   /* Quad-word storage */
typedef float                       Float;   /* Single-precision floating point */
typedef double                      Real;    /* Double-precision floating point */

/* other useful typedefs */
typedef U32                         Msec;    /* Milliseconds */
typedef U64                         Usec;    /* Microseconds */
typedef U32                         Flags;   /* 32 flags */
typedef S8*                         CStr;    /* C-style string pointer */

/* C code boolean */
typedef U8                          Bool;
#define TRUE                        1           /* TRUE value for C code */
#define FALSE                       0           /* FALSE value for C code */

#define NULL                        0           /* NULL */
#define NUL                        '\0'         /* NUL  */

#define OK                          0           /* Success */
#define ERR                         -1          /* Failure */

/* max and mins */
static const U64 MAX_UINT64 = 18446744073709551615ULL;  /* Max unsigned 64-bit value */
static const S64 MAX_INT64  = 9223372036854775807LL;    /* Max signed 64-bit value */
static const S64 MIN_INT64  = -9223372036854775807LL;   /* Min signed 64-bit value */
static const U32 MAX_UINT   = 4294967295UL;             /* Max unsigned 32-bit value */
static const S32 MAX_INT    = 2147483647;               /* Max signed 32-bit value */
static const S32 MIN_INT    = -2147483647;              /* Min signed 32-bit value */
static const U16 MAX_USHORT = 65535;                    /* Max unsigned 16-bit value */
static const S16 MAX_SHORT  = 32767;                    /* Max signed 16-bit value */
static const S16 MIN_SHORT  = -32767;                   /* Min signed 16-bit value */
static const U8  MAX_UCHAR  = 255;                      /* Max unsigned 8-bit value */
static const S8  MAX_CHAR   = 127;                      /* Max signed 8-bit value */

/*****************************************************************************
**  Public Data Prototypes 
*****************************************************************************/

/*****************************************************************************
**  Public Function Prototypes 
*****************************************************************************/
 
#ifdef __cplusplus
}
#endif 
 
#endif
