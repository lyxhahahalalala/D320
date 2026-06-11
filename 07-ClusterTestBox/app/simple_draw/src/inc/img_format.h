/*
****************************************************************************
PROJECT : GRAPE_APP
FILE    : $Id: img_format.h 25427 2013-06-14 07:55:45Z nan.wang $
============================================================================ 
DESCRIPTION
Image format
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

#ifndef _IMG_FORMAT_H_
#define _IMG_FORMAT_H_


#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: 2D Image Format Definition
  
  This header describes the 2D image format used by the
  framework.
*/

/***********************************************************
  Section: Global Types
*/

/***********************************************************
  Enum: Img_ColorFormat_t

  Image color format type.

  Values:
    IMG_ALPHA8     - 8bit Alpha Format
    IMG_RGB565     - RGB565 Format
    IMG_ARGB8888   - ARGB8888 Format
    IMG_RGBA8888   - RGBA8888 Format
    IMG_ARGB6666   - ARGB6666 Format
    IMG_RGBA6666   - RGBA6666 Format
    IMG_ARGB1555   - ARGB1555 Format
    IMG_RGB888     - RGB888 Format
    IMG_RGBX8888   - RGBX8888 Format
    IMG_RGBA5551   - RGBA5551 Format
    IMG_ARGB4444   - ARGB4444 Format
    IMG_RGBA4444   - RGBA4444 Format
    IMG_CLUT8      - CLUT8 Format

  additional values used as flags can be stored together with previous modes:
  IMG_MODE_RLE   - Format is RLE decoded
*/

typedef enum {
    IMG_ALPHA8,
    IMG_RGB565,
    IMG_ARGB8888,
    IMG_RGBA8888,
    IMG_ARGB6666,
    IMG_RGBA6666,
    IMG_ARGB1555,
    IMG_RGB888,
    IMG_RGBX8888,
    IMG_RGBA5551,
    IMG_ARGB4444,
    IMG_RGBA4444,
    IMG_CLUT8
} Img_ColorFormat_t;


/***********************************************************
  Enum: Img_Attributes_t

  Attribute flags of the image. (bit description of image_t->attributes variable)
  The attribute flags can be stored together.

  Values:
  IMG_ATTRIBUTE_IS_RLE_COMPRESSED           - bit 0
                                                ==0 texture is RAW,
                                                ==1 texture is RLE compressed
  IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY  - bit 1
                                                ==0 allow copy data to video memory before usage
                                                ==1 use data directly from persistent memory
  IMG_ATTRIBUTE_SWIZZLE                     - bit 2
                                                ==0 bitmap data is not swizzled
                                                ==1 bitmap data is swizzled (depends on DHD Swizzling mode)
  IMG_ATTRIBUTE_CLUT                        - bit 3
                                                ==0 bitmap contains color data
                                                ==1 bitmap contains color table indices
  IMG_ATTRIBUTE_RLE_DECOMPRESS              - bit 4
                                                ==0 decompress in drawing engine
                                                ==1 decompress in software before usage
*/

typedef enum {
    IMG_ATTRIBUTE_RLE_COMPRESSED                = (1 << 0),
    IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY    = (1 << 1),
    IMG_ATTRIBUTE_SWIZZLE                       = (1 << 2),
    IMG_ATTRIBUTE_CLUT                          = (1 << 3),
    IMG_ATTRIBUTE_RLE_DECOMPRESS                = (1 << 4),
    IMG_ATTRIBUTE_UNUSED                        = (1 << 5),
} Img_Attributes_t;


/***********************************************************
  Type: Img_t

  The structure contains all data of a texture.

  Members:
  Name             - Name of the image or 0
  PalName          - Name of the palette or 0
  Width            - Width of the texture
  Height           - Height of the texture
  Colorformat      - Drawing engine color format
  Attributes       - collection of image attributes (see Img_Attributes_t)
  Index            - Pointer to index table
  IndexLength      - Length of the index table in bytes
  IndexEntrys      - Number of entries in the table
  Data             - Pointer to image data / pointer to image in VRAM
  DataLength       - Image data length
*/

typedef struct {
    const char             *Name;
    const char             *PalName;
    uint32_t                Width;
    uint32_t                Height;
    Img_ColorFormat_t       Colorformat;
    uint32_t                Attributes;
    uint8_t               **Index;
    uint32_t                IndexLength;
    uint32_t                IndexEntrys;
    uint8_t               **Data;
    uint32_t                DataLength;
} Img_t;


#ifdef __cplusplus
}
#endif

#endif /* _IMG_FORMAT_H_ */

