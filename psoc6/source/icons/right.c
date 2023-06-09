/*********************************************************************
 * This file is created by LCD Bitmap Converter for emWin v2.0       *
 *                         Pixfonter Technology.                     *
 *                         http://www.pixfonter.com/                 *
 *                         support@pixfonter.com                     *
 *********************************************************************
 * Source file:  right                                               *
 * Dimensions:   24 x 24                                             *
 * Bit counts:   8 bpp                                               *
 * Colors:       82                                                  *
 *********************************************************************/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
#define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmright;

/*	Palette
    The following are the entries of the palette table.
    Every entry is a 32-bit value (of which 24 bits are actually used)
    xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    |no use| | Blue | | Green| |  Red |
*/

static GUI_CONST_STORAGE GUI_COLOR _Colorsright[82] = {
    0x000000, 0xBFBFBF, 0xCCCCCC, 0xCFCFCF, 0xD0D0D0, 0xD3D3D3, 0xD4D4D4, 0xD7D7D7, 0xD8D8D8,
    0xDBDBDB, 0xDFDFDF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palright = {82, /* number of entries */
                                                     0,  /* No transparency */
                                                     &_Colorsright[0]};

/*	Bitmap data	*/
static GUI_CONST_STORAGE unsigned char _acright[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Line: 1 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, /* Line: 2 */
    0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x05, 0x04,
    0x06, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, /* Line: 3 */
    0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x05, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x05, 0x00, 0x00, 0x00, /* Line: 4 */
    0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, /* Line: 5 */
    0x00, 0x04, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x08, 0x00, /* Line: 6 */
    0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x06,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, /* Line: 7 */
    0x05, 0x06, 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x06,
    0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x06, 0x05, /* Line: 8 */
    0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06,
    0x06, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, /* Line: 9 */
    0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
    0x06, 0x06, 0x06, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x06, /* Line: 10 */
    0x06, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x06, /* Line: 11 */
    0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x06, /* Line: 12 */
    0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x06, 0x06, /* Line: 13 */
    0x06, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x06, /* Line: 14 */
    0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
    0x06, 0x06, 0x06, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, /* Line: 15 */
    0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06,
    0x06, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, /* Line: 16 */
    0x05, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x06,
    0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x05, /* Line: 17 */
    0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x06,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, /* Line: 18 */
    0x00, 0x04, 0x06, 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x07, 0x00, /* Line: 19 */
    0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, /* Line: 20 */
    0x00, 0x00, 0x00, 0x05, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, /* Line: 21 */
    0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x05, 0x06,
    0x06, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, /* Line: 22 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, /* Line: 23 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /* Line: 24 */
};

GUI_CONST_STORAGE GUI_BITMAP bmright = {
    24,        /* width */
    24,        /* height */
    24,        /* bytes per line */
    8,         /* bits per pixel */
    _acright,  /* Pointer to bitmap data */
    &_Palright /* Pointer to palette */
};

/********** End of file **********/