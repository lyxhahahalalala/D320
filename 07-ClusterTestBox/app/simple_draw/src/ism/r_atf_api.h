/*
****************************************************************************
PROJECT : Application test framework
FILE    : $Id: r_atf_api.h 4774 2015-02-10 16:48:54Z matthias.nippert $
============================================================================ 
Description
API definition for the application test framework
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2012
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************

*/

#ifndef ATF_API_H_
#define ATF_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title:  ATF API

  The Application Test Framework API provides methods to handle application tests
  or similar tasks. 
  
  It supports automatic execution for a user defined list of functions. 
  
  Features: 
  - Main Menu handler with sub menu supports
  - Automated functions list execution   with result supervision 
  - Simple selection menu handler 

*/
/*******************************************************************************
  Section: Global Macros
*/
/*******************************************************************************
  Macro: R_ATF_VOID
  
  Cast an function returning a value to void 
  
  This can be used to "shorten" the cast automated functions (that return a 
  value of type <r_Error_t>) for usage in the "standard" ATF menu. 
*/


#define R_ATF_VOID (void (*)(void))

/*******************************************************************************
  Section: Global Constants
*/

/*******************************************************************************
  Constant: R_ATF_NO_RETRY
  
  Placeholder value signalling that automatic execution shall halt on error.
  
  If during automated execution an error occurs(a function call returns an error) 
  the processing will continue with a specified menu entry or not at all if this
  constant is used.    
*/

#define R_ATF_NO_RETRY 0xFFFFu

/*******************************************************************************
  Section: Global Types
*/
/* because I want a code example within a comment */
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

/*******************************************************************************
  Type: r_atf_Menu_t

  Definition of one "row" of a menu array.
  
  The ATF main menu is an array of objects of this type. 
  
  Members:
  item_id              - Character used for item selection (user choice).  
                         Characters are numbers 1...9u upper or lower case letters w/o "x".
  menu_id              - ID (number) of the menu, 0=root 
  child_id             - ID (number)of the child for the current entry, 0=no child
  item_desc            - Description of item (string)
  item_function        - Function vector for the given item 

  Example: 
  >  const r_atf_Menu_t test_cases[]=
  >  {
  >     item_id  menu_id    child_id      item_desc         item_function 
      
      *This is the main menu* 
  >    {(int8_t)'1'      , 0u,        0u,         "Root 1",             root_1       },
  >    {(int8_t)'2'      , 0u,        0u,         "Root 2",             root_2       },
  >    {(int8_t)'3'      , 0u,        0u,         "Root 3",             root_3       },
  >    {(int8_t)'4'      , 0u,        1u,         "Test Unit A ",       prepare_sub_a},
  >    {(int8_t)'5'      , 0u,        1u,         "Test Unit B",        prepare_sub_b},
  >    {(int8_t)'6'      , 0u,        2u,         "Another Sub ",       0u   },
      *This is the common sub menu for Unit A and B*  
  >    {(int8_t)'a'      , 1u,        0u,         "Item A",             item_a       },
  >    {(int8_t)'b'      , 1u,        0u,         "Item B",             item_b       },
  >    {(int8_t)'c'      , 1u,        0u,         "Item C",             item_c       },
      *This is sub menu 2 (Another Sub)*
  >    {(int8_t)'a'      , 2u,        0u,         "Item X",             item_x       },
  >    {(int8_t)'b'      , 2u,        0u,         "Item Y",             item_y       },
  >    {(int8_t)'1'      , 2u,        3u,         "Sub A-A ",           0u            },                                     
     *This is sub menu of  sub menu 2 (Sub A-A)*
  >    {(int8_t)'a'      , 3u,        0u,         "Another Item A",     item_aa      },
  >    {(int8_t)'b'      , 3u,        0u,         "Another Item B",     item_ab      },
  >    {(int8_t)'c'      , 3u,        0u,         "Another Item C",     item_ac      },
      *Delimiter, don't change *                                           
  >    {(int8_t)'0'      , 0u,        0u,       0u,                    0u              }
  > };
*/
#if defined (__ghs__)
#pragma ghs endnomisra
#endif /* __ghs */


typedef struct {
    int8_t     item_id;            
    uint8_t    menu_id;
    uint8_t    child_id;
    char       *item_desc;        
    void      (*item_function)(void);
} r_atf_Menu_t;

/* because I want a code example within a comment */
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

/*******************************************************************************
  Type: r_atf_Automatic_t

  Definition of one "row" of a menu array for automatic execution 
  
  The usual use case is to define a constant array (table) of test functions
  which wil be automatically executed in the order of their appearance in the array.   

  Members:
  Desc              - Description of the item (string)
  ContAfterError    - What to do if the current function returns an error? 
                      When n = R_ATF_NO_RETRY abort test execution else 
                     restart execution (processing of the test table) 
                      at index n 
  Func              - Function vector for the given item  
    

  Example: 
  > const r_atf_Automatic_t test_auto[]=
  > {
  >        Desc      ContAfterError            Func       
  >     {"Root 1",        0                 ,  root_1       },
  >     {"Root 2",        R_ATF_NO_RETRY    ,  root_2       },
  >     {"Root 3",        3                 ,  root_3       },
  >     {"Test Unit A ",  4                 ,  prepare_a    },
  >     {"Test Unit B",   0                 ,  exec_a       },
       *Delimiter, don't change *
  >     {""           ,   0                 ,  0            }
  > };
*/
#if defined (__ghs__)
#pragma ghs endnomisra
#endif /* __ghs */

typedef struct {
    char        *Desc;        
    uint16_t    ContAfterError;
    r_Error_t   (*Func)(void);
} r_atf_Automatic_t;





/*******************************************************************************
  Type: r_atf_MenuItem_t

  Definition of one "row" of a simple menu (selection list) 
  
  This type is used to define indexed item lists. These can be passed to a function
  for unified selection handling.   
  
  Members:
  item_desc      - Description of the menu item (string)
*/

typedef struct 
{
        int8_t        *item_desc;        
} r_atf_MenuItem_t; 


/*******************************************************************************
  Type: r_atf_ValueItem_t
  
  Definition of one "row" of a selection menu (selection list).  

  This strucuture is used for simple menus allowing to choose from a set 
  of possible values.

  Members:
  item_value     - value of menu item 
  item_desc      - Description of menu item (string)
*/

typedef struct 
{
        int32_t     item_value;
        int8_t        *item_desc;        
} r_atf_ValueItem_t;


/*******************************************************************************
  Section: Application Test Framework Functions
*/

/*******************************************************************************
  Function: R_ATF_DevInit

  Init the device used with the application.

  This function must be implemented in a file located in the test application's 
  config directory (sub-directory 'config' located in the application's target 
  directory). 
  
  This is the first function call in main().
   
  Parameters:
  void
    
  Returns:
  void
*/  
void R_ATF_DevInit(void);



/*******************************************************************************
  Function: R_ATF_TestInit

  Device _independent_ initialisation of the application 

  This function must be implemented in the atf application file. 
  This is the framework init routine - the second call in main().
  
  At least the pointers to the menus (main and/or automatic) should be assigned there. 
  
  Parameters:
  void
    
  Returns:
  void
*/  
void R_ATF_TestInit(void);


/*******************************************************************************
  Function: R_ATF_WaitForChar

  Wait for the input of the given character. 

  Parameters:
  Character       - Character to be checked 
  
  Returns:
  void
*/

void R_ATF_WaitForChar(int8_t character);


/*******************************************************************************
  Function: R_ATF_Confirm

  Print a message to confirm and
  wait for the input of any the given characters.

  Parameters:
  Message         - Message to be confirmed.
  PositiveChars   - Characters to be interpreted as positive answer.
                    Set to NULL to use default characters "y" and "Y".
  NegativeChars   - Characters to be interpreted as negative answer.
                    Set to NULL to use default characters "n" and "N".
  
  Returns:
  1 for positive answers, 0 for negative answers.
*/

int8_t R_ATF_Confirm(char * Message, char * PositiveChars, char * NegativeChars);


/*******************************************************************************
  Function: R_ATF_Prologue

  Prints the given description string with standard (unified) formatting.
  
  In case the Description is 0u, the coresponding test menu entry's description 
  string will be used. 

  Parameters:
  Description       - Output string or 0
  
  Returns:
  void
*/

void R_ATF_Prologue(char * Description);


/*******************************************************************************
  Function: R_ATF_Epilogue

  Prints a standard epilogue and waits for input of a 'x' 

  Parameters:
  void
    
  Returns:
  void
*/

void R_ATF_Epilogue(void);


/*******************************************************************************
  Function: R_ATF_Control

  Outputs user instructions (given description) and waits for the given character.

  Parameters:
  Character         - Required Character
  Description       - Output string
  
  Returns:
  void
*/

void R_ATF_Control(char Character, char * Description);


/*******************************************************************************
  Function: R_ATF_MenuSelect

  Display a simple selection menu.

  Print32_t the given list of items, wait for input and return user's choice.  
  
  Parameters:
  MenuItems     - menu items list,  see <r_atf_MenuItem_t>

    
  Returns:
  Array index according to choosen item
*/  

uint8_t R_ATF_MenuSelect(const r_atf_MenuItem_t * MenuItems);

/*******************************************************************************
  Function: R_ATF_ValueSelect

  Function for simple value selection.

  Display the given list of descriptions, wait for user input and return the 
  chosen value. 
 
  
  Parameters:
  ValueItems     - value items lsit , see <r_atf_ValueItem_t>
    
  Returns:
  value assigned to the choosen item
*/  

uint32_t R_ATF_ValueSelect(const r_atf_ValueItem_t * ValueItems);



/*******************************************************************************
  Function: R_ATF_SetMenu

  Pass the application's main menu to the framework 

  Parameters:
  TestMenu  - pointer to test menu, see <r_atf_Menu_t>

  Returns:
  void  
*/
void R_ATF_SetMenu(const r_atf_Menu_t * TestMenu);


/*******************************************************************************
  Function: R_ATF_SetMenuPreamble
  
  Set an informative text regarding this test application. 
  To be printed below the menu header.
  
  Parameters:
  MenuPreamble - pointer to string
  
  Returns:
  void
*/
void R_ATF_SetMenuPreamble(char * MenuPreamble);


/*******************************************************************************
  Function: R_ATF_SetAutomatic

  Pass the applications automatic function table to the framework 

  Parameters:
  Auto        - pointer to the table of functions, see <r_atf_Automatic_t>
  AutoStart   - if AutoStart !=0 then the automatic test will be executed 
                after the main menu was finished 
                If the main menu is not present (because only automatic execution is 
                configured), this parameter has no meaning  
  Returns:
  void  -
*/
void R_ATF_SetAutomatic(const r_atf_Automatic_t * Auto, uint8_t AutoStart);  


/*******************************************************************************
  Function: R_ATF_SetAutoConfig

  Pass the applications configuration function for automatic execution 
  to the framework 

  Parameters:
  AutoConf          - pointer to the user configuration function 
  Returns:
  void  -
*/
void R_ATF_SetAutoConfig(void (*AutoConf) (void));  

/*******************************************************************************
  Function: R_ATF_Automatic

 Handle the automatic function array. 
 
 Execute all functions "listed" in  the loc_Auto array, see <r_atf_Automatic_t>
 Provides some basic error handling 
 - halt or continue on error
 - count/display total errors
   
  Parameters:
  void  -

  Returns:
  void  -
*/
void R_ATF_Automatic(void) ; 


#ifdef __cplusplus                                   
}                                                    
#endif

#endif /* ATF_API*/
