/*----------------------------------------------------------------------------/
/  FatFs - Generic FAT Filesystem module  R0.13c                              /
/-----------------------------------------------------------------------------/
/
/ Copyright (C) 2018, ChaN, all right reserved.
/
/ FatFs module is an open source software. Redistribution and use of FatFs in
/ source and binary forms, with or without modification, are permitted provided
/ that the following condition is met:

/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright owner or contributors be NOT LIABLE for any damages caused
/ by use of this software.
/
/----------------------------------------------------------------------------*/


#ifndef FF_DEFINED
#define FF_DEFINED  86604   /* Revision ID */

#define FF_CODE_PAGE        936
#define FF_USE_LFN          16

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned int    UINT;   /* int must be 16-bit or 32-bit */
typedef unsigned char   BYTE;   /* char must be 8-bit */
typedef unsigned short  WORD;   /* 16-bit unsigned integer */
typedef unsigned short  WCHAR;  /* 16-bit unsigned integer */
typedef unsigned int    DWORD;	/* 32-bit unsigned integer */


WCHAR ff_oem2uni (WCHAR oem, WORD cp);  /* OEM code to Unicode conversion */
WCHAR ff_uni2oem (DWORD uni, WORD cp);  /* Unicode to OEM code conversion */
DWORD ff_wtoupper (DWORD uni);          /* Unicode upper-case conversion */



#ifdef __cplusplus
}
#endif

#endif /* FF_DEFINED */
