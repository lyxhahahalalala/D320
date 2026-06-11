
#ifndef   TYPES_H
#define   TYPES_H

#include "r_typedefs.h"
/*
*******************************************************************************
**  Global Typedefinitions
*******************************************************************************
*/

typedef unsigned long long uint64;
typedef float          float32;
typedef double         float64;



typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;

typedef char  sint8;
typedef short sint16;
typedef long  sint32;

typedef signed char  int8;
typedef signed short int16;
typedef signed long  int32;

typedef unsigned char  bit8;
typedef unsigned short bit16;
typedef unsigned long  bit32;

typedef unsigned int   uint;
typedef signed int     sint;

#ifndef NULL
#define NULL (0)
#endif

#ifndef TRUE                                /* conditional check */
#define TRUE  1
#endif

#ifndef FALSE                                            /* conditional check */
#define FALSE 0
#endif


/* limits of data types */
#define MAXUINT8        (0xff)
#define MINUINT8        (0x0)
#define MAXSINT8        (0x7f)
#define MINSINT8        (-(MAXSINT8) - 1)
#define MAXUINT16       (0xffff)
#define MINUINT16       (0x0)
#define MAXSINT16       (0x7fff)
#define MINSINT16       (-(MAXSINT16) - 1)
#define MAXUINT32       (0xfffffffful)
#define MINUINT32       (0x0ul)
#define MAXSINT32       (0x7fffffffl)
#define MINSINT32       (-(MAXSINT32) - 1l)
#define MAXREAL32       (3.40282347E38F)
#define MINREAL32       (1.17549435E-38F)
#define MAXREAL64       (1.7976931348623157E308)
#define MINREAL64       (2.2250738585072016E-308)

/* compatibility defines for not UBK conform software: ASCET */
/* these defines should be removed as fast as possible */
#define UINT8_MAX       MAXUINT8
#define UINT8_MIN       MINUINT8
#define SINT8_MAX       MAXSINT8
#define SINT8_MIN       MINSINT8
#define UINT16_MAX      MAXUINT16
#define UINT16_MIN      MINUINT16
#define SINT16_MAX      MAXSINT16
#define SINT16_MIN      MINSINT16
#define UINT32_MAX      MAXUINT32
#define UINT32_MIN      MINUINT32
#define SINT32_MAX      MAXSINT32
#define SINT32_MIN      MINSINT32

#define b_BBasT         uint8
#define w_BBasT         uint16
#define l_BBasT         uint32

#define b_MASK          (uint8)0xFFu
#define w_MASK          (uint16)0xFFFFu
#define l_MASK          (uint32)0xFFFFFFFFul

/* definition of bit masks */
#define MBIT0           (0x00000001)
#define MBIT1           (0x00000002)
#define MBIT2           (0x00000004)
#define MBIT3           (0x00000008)
#define MBIT4           (0x00000010)
#define MBIT5           (0x00000020)
#define MBIT6           (0x00000040)
#define MBIT7           (0x00000080)
#define MBIT8           (0x00000100)
#define MBIT9           (0x00000200)
#define MBITA           (0x00000400)
#define MBITB           (0x00000800)
#define MBITC           (0x00001000)
#define MBITD           (0x00002000)
#define MBITE           (0x00004000)
#define MBITF           (0x00008000)
#define MBIT10          (0x00010000)
#define MBIT11          (0x00020000)
#define MBIT12          (0x00040000)
#define MBIT13          (0x00080000)
#define MBIT14          (0x00100000)
#define MBIT15          (0x00200000)
#define MBIT16          (0x00400000)
#define MBIT17          (0x00800000)
#define MBIT18          (0x01000000)
#define MBIT19          (0x02000000)
#define MBIT1A          (0x04000000)
#define MBIT1B          (0x08000000)
#define MBIT1C          (0x10000000)
#define MBIT1D          (0x20000000)
#define MBIT1E          (0x40000000)
#define MBIT1F          (0x80000000)


#endif  /* TYPES_H */
