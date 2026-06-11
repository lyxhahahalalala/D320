#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: fcl_user.h $
* Mod. Revision : $Revision: 1.1 $
* Mod. Date     : $Date: 2013/03/27 10:05:44MEZ $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : Sample application functions to prepare Self-Programming
************************************************************************************************************/

#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_ON 0292
#endif

/************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 3.1 (QAC message 0292)
* Reason:       To support automatic insertion of revision, module name etc. by the source 
*               revision control system it is necessary to violate the rule, because the 
*               system uses non basic characters as placeholders.
* Verification: The placeholders are used in commentars only. Therefore rule violation cannot 
*               influency code compilation.
************************************************************************************************************/
  
#ifndef FCLUSER_H
#define FCLUSER_H


/************************************************************************************************************
Exported global functions (to be accessed by other files) 
************************************************************************************************************/

/* Enable Flash modification by the FCL */
void FCLUser_Open (void);

/* Disable Flash modification by the FCL */
void FCLUser_Close (void);

#endif /* end of FCLUSER_H */
