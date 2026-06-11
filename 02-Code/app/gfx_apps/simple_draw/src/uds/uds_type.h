/***************************************************************************//**
    \file          uds_type.h
    \author        
    \mail          
    \version       0
    \date         2020-03-26
    \description   uds type
*******************************************************************************/
#ifndef _UDS_TYPE_H_
#define _UDS_TYPE_H_

/*******************************************************************************
    Include Files
*******************************************************************************/

/*******************************************************************************
    Type Definition
*******************************************************************************/
/**
	\def TRUE
	Bool value true.
*/
#ifndef TRUE
#define	TRUE	(1)
#endif

/**
	\def FALSE
	Bool value false.
*/
#ifndef FALSE
#define FALSE	(0)
#endif
/**
	\def NULL
	NULL type.
*/
#ifndef NULL
#define NULL  ((void *) 0)
#endif
/**
	\typedef unsigned char bool_t
	Bool type.
*/
typedef unsigned char   bool_t;
// typedef unsigned short  uint16_t;
// typedef unsigned char   uint8_t;
// typedef unsigned int    uint32_t;

#endif
/****************EOF****************/
