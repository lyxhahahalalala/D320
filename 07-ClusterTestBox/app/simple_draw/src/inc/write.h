/*
****************************************************************************
PROJECT : GRAPE_APP
FILE    : $Id: write.h 25427 2013-06-14 07:55:45Z nan.wang $
============================================================================ 
DESCRIPTION
Write Module Interface
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2014
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER 

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer’s premises listed in the signed license
agreement.
****************************************************************************
*/

#ifndef _WRITE_H_
#define _WRITE_H_

#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "img_format.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Write Module Interface
  
  This header is the interface to the DRW2D based bitmap
  write functions.
*/


/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Type: FontPos_t

  Character position and width information.

  The type is used to specify the position of a character
  inside a font texture.

  Members:
  Offset  - Pixel position in the texture
  Width   - Pixel width in the texture
*/

typedef struct {
    int Offset;
    int Width;
} FontPos_t;

/***********************************************************
  Type: Font_t

  Font data type.

  This structure contains the data of a font.

  Members:
  Texture - Pointer to the font's image structure
  Pos     - Array of character information
  Height  - Pixel height of the font
*/

typedef struct {
    Img_t           *Texture;
    const FontPos_t *Pos;
    int              Height;
} Font_t;

/***********************************************************
  Function: SelectFont

  Select font for operation.

  The 'write' module uses an internal pointer to the font,
  which is used by any text operation. This function sets
  that pointer.

  Parameters:
  Font       - Font to be used

  Returns:
  void
*/

void SelectFont(const Font_t *Font);


/***********************************************************
  Function: GetFontHeight

  Get height of the font.

  The function returns the height in pixel of the selected
  font.

  Parameters:
  void

  Returns:
  Height of font.
*/

int GetFontHeight(void);


/***********************************************************
  Function: GetCharWidth

  Get character width.

  The function returns the width of a character in pixel
  when using the currently selected font.

  Parameters:
  Character  - ASCII value of the character

  Returns:
  Character width in pixel.
*/

int GetCharWidth(char Character);


/***********************************************************
  Function: GetTextWidth

  Return width of a text.

  The function returns the width of a text in pixel
  when using the currently selected font.

  Parameters:
  Text       - Pointer to a 0 terminated string

  Returns:
  Text width in pixel
*/

int GetTextWidth(char *Text);


/***********************************************************
  Function: GetTextHeight

  Return height of a text.

  The function returns the height of a text in pixel
  when using the currently selected font.

  Parameters:
  Text       - Pointer to a 0 terminated string

  Returns:
  Text height in pixel
*/

int GetTextHeight(char *Text);


/***********************************************************
  Function: Write

  Write a text.

  Use the currently selected buffer, font and color to draw
  a text at the given position.

  Parameters:
  Dev        - R_DRW2D handle
  PosX       - Display X position
  PosY       - Display Y position
  Color      - Color to multiply with the font texture.
  Text       - Pointer to a 0 terminated string

  Returns:
  void
*/

void Write(
    r_drw2d_Device_t Dev,
    int              PosX,
    int              PosY,
    r_drw2d_Color_t  Color,
    char             *Text
);
 
#ifdef __cplusplus
}
#endif

#endif /* _WRITE_H_ */

