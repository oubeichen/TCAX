/* 
 *  Copyright (C) 2009-2011 milkyjing <milkyjing@gmail.com>
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef TCAXLIB_PIX_H
#define TCAXLIB_PIX_H
#pragma once

#include <stdint.h>
#include "std.h"

#define MATH_PI 3.14159265358979323846264338327950288

#define MAKE8B8B8B(r,g,b) ((unsigned long)((unsigned char)(r) | ((unsigned short)((unsigned char)(g)) << 8) | ((unsigned long)((unsigned char)(b)) << 16)))
#define MAKERGB MAKE8B8B8B


/**
 * TCAX_Pix structure used to store pixel info of text(s) / image(s).
 */
typedef struct _tcax_pix {
    double initX;    /**< left most */
    double initY;    /**< upper most */
    int width;     /**< width of the PIX */
    int height;    /**< height of the PIX */
    int size;    /**< size = height * (width * 4) */
    unsigned char *buf;    /**< pixel values of the PIX */
} TCAX_Pix, *TCAX_pPix;

/**
 * TCAX_Points structure is an auto increase list.
 */
typedef struct _tcax_points {
    double *xBuf;
    double *yBuf;
    unsigned char *aBuf;
    int count;       /**< number of points */
    int capacity;    /**< maximun points that the buffer can contain */
} TCAX_Points, *TCAX_pPoints;

/**
 * TCAX_PyPix structure that has the following structure ((initX, initY), (width, height), (a1, a2, a3, ...)).
 */
typedef py::tuple TCAX_PyPix;

/**
 * TCAX_PyBigPix structure that has the following structure [(PIX, offset_x, offset_y, layer), (PIX, offset_x, offset_y, layer), ...].
 */
typedef py::list TCAX_PyBigPix;

/**
 * TCAX_PyPoints structure a tuple of tuples (points).
 * ((x1, y1, a1), (x2, y2, a2), ...)
 */
typedef py::tuple TCAX_PyPoints;

/**
 * Clip a number into 0 to 255 inclusive
 */
extern unsigned char CLIP_0_255(int a);

/**
 * pPoints->xBuf and pPoints->yBuf need to be allocated before being passed to the function, and it will be reallocated if more space is needed
 * pPoints->count and pPoints->capacity should also be specified before being passed to the function, and their value may be modified
 */
extern void tcaxlib_points_append(TCAX_pPoints pPoints, double x, double y, unsigned char a);

/**
 * Convert TCAX PIX to TCAX PY PIX.
 * @param pPix pointer to TCAX_Pix structure
 * @param delPix indicates whether to delete PIX before returning of the function
 * @return TCAX_PyPix
 */
extern TCAX_PyPix tcaxlib_convert_pix(const TCAX_pPix pPix, int delPix);

/**
 * Convert TCAX PY PIX to TCAX PIX. 
 * Remark: in the following functions we will just use PIX, it may stand for TCAX PIX or TCAX PY PIX, in case that there is no ambiguity.
 *
 * @param pyPix the TCAX_PyPix structure
 * @param pPix pointer to TCAX_Pix structure that is going to hold the converted data
 * @return const TCAX_pPix
 */
extern TCAX_pPix tcaxlib_convert_py_pix(const TCAX_PyPix &pyPix, TCAX_pPix pPix);

/**
 * Get points from TCAX PY PIX.
 * @param PIX
 * @return TCAX_PyPoints
 */
//PixPoints(PIX), where "PIX" is tuple
extern TCAX_PyPoints tcaxlib_pix_points(TCAX_PyPix &PIX);

/**
 * Create a blank PIX.
 * @param width
 * @param height
 * @param rgba
 * @return TCAX_PyPix
 */
//BlankPix(width, height, rgba)
extern TCAX_PyPix tcaxlib_create_blank_pix(int width, int height, uint32_t rgba);

/**
 * Resample TCAX PIX. 
 * Remark: the target width and height should be specified in the target TCAX_Pix structure before it is sent to the function. 
 * Other variables will be ignored. Note that the `buf' of the target TCAX_Pix structure should not be allocated before it is 
 * sent to the function, otherwise will cause memory leak.
 *
 * @param pPixSrc pointer to source TCAX_Pix structure
 * @param pPixDst pointer to target TCAX_Pix structure
 * @return TCAX_pPix
 */
extern TCAX_pPix tcaxlib_resample_pix(const TCAX_pPix pPixSrc, TCAX_pPix pPixDst);

/**
 * supports fractional width and height
 */
extern TCAX_pPix tcaxlib_resample_pix_ex(const TCAX_pPix pPixSrc, TCAX_pPix pPixDst, double width, double height);

/**
 * Resample TCAX PY PIX.
 * @param PIX
 * @param width
 * @param height
 * @return TCAX_PyPix
 */
//PixResize(PIX, width, height)
extern TCAX_PyPix tcaxlib_resample_py_pix(TCAX_PyPix &PIX, int width, int height);

/**
 * supports fractional width and height
 */
//PixResizeF(PIX, width, height)
extern TCAX_PyPix tcaxlib_resample_py_pix_ex(TCAX_PyPix &PIX, double width, double height);

/**
 * Applay a texture to PIX.
 * @param PIX
 * @param Texture
 * @return TCAX_PyPix
 */
//PixTexture(PIX, texture)
extern TCAX_PyPix tcaxlib_apply_pix_texture(TCAX_PyPix &PIX, TCAX_PyPix &texture);

/**
 * Applay a mask to PIX.
 * @param PIX
 * @param mask
 * @return TCAX_PyPix
 */
//PixMask(PIX, mask)
extern TCAX_PyPix tcaxlib_apply_pix_mask(TCAX_PyPix &PIX, py::tuple &mask);

/**
 * Replace the alpha channel of the source PIX with the alpha channel of the destination PIX. 
 * Remark: the transparent pixels of the source PIX will stay unchanged.
 *
 * @param self reserved
 * @param PIX
 * @param PIX_alpha
 * @return TCAX_PyPix
 */
//PixReplaceAlpha(PIX, PIX_alpha)
extern TCAX_PyPix tcaxlib_pix_replace_alpha(TCAX_PyPix &PIX, TCAX_PyPix &PIX_alpha);

/**
 * Enlarge a PIX. 
 * Remark: the PIX is put in the middle of the enlarged one, and the extra space is filled with 0. 
 * The target width and height should be specified in the target TCAX_Pix structure before it is sent to the function. 
 * And target width and height should be both larger than (or equal to) source ones. 
 * Other variables will be ignored. Note that the `buf' of the target TCAX_Pix structure should be empty before it is sent to the function, 
 * otherwise will cause memory leaking.
 *
 * @param pPixSrc pointer to source TCAX_Pix structure
 * @param pPixDst pointer to target TCAX_Pix structure
 * @return TCAX_pPix
 */
extern TCAX_pPix tcaxlib_enlarge_pix(const TCAX_pPix pPixSrc, TCAX_pPix pPixDst);

/**
 * Enlarge a TCAX PY PIX.
 * @param PIX
 * @param x
 * @param y
 * @return TCAX_PyPix
 */
//PixEnlarge(PIX, x, y)
extern TCAX_PyPix tcaxlib_enlarge_py_pix(TCAX_PyPix &PIX, int x, int y);

/**
 * Crop a TCAX PY PIX.
 * @param PIX
 * @param left
 * @param top
 * @param right
 * @param bottom
 * @return TCAX_PyPix
 */
//PixCrop(PIX, left, top, right, bottom)
extern TCAX_PyPix tcaxlib_crop_py_pix(TCAX_PyPix &PIX, int left, int top, int right, int bottom);

/**
 * Strip blank border of a TCAX PY PIX.
 * @param PIX
 * @return TCAX_PyPix
 */
//PixStrip(PIX)
extern TCAX_PyPix tcaxlib_py_pix_strip_blank(TCAX_PyPix &PIX);

/**
 * Applay Gauss IIR blur to a PIX.
 * @param PIX
 * @param radius
 * @return TCAX_PyPix
 */
//PixBlur(PIX, radius)
extern TCAX_PyPix tcaxlib_apply_gauss_blur(TCAX_PyPix &PIX, int radius);

/**
 * Combine two PIXs, i.e., overlay one PIX upon the other.
 * Remark: during overlaying, the float of the input PIXs' initX and initY will 
 * be discarded and the result PIX's initX and initY will be integers, 
 * so, use BilinearFilter to remove the float of the input PIXs if necessary.
 *
 * @param back
 * @param overlay
 * @return py::tuple
 */
//CombinePixs(back, overlay)
extern TCAX_PyPix tcaxlib_combine_two_pixs(TCAX_PyPix &back, TCAX_PyPix &overlay);

/**
 * Change the color of a PIX by multiplying factors to each channel of RGBA.
 * @param PIX
 * @param r_f
 * @param g_f
 * @param b_f
 * @param a_f
 * @return TCAX_PyPix
 */
//PixColorMul(PIX, r_f, g_f, b_f, a_f)
extern TCAX_PyPix tcaxlib_pix_color_multiply(TCAX_PyPix &PIX, double r_f, double g_f, double b_f, double a_f);

/**
 * Change the color of a PIX by shifting values of each channel of RGBA.
 * @param PIX
 * @param r
 * @param g
 * @param b
 * @param a
 * @return TCAX_PyPix
 */
//PixColorShift(PIX, r, g, b, a)
extern TCAX_PyPix tcaxlib_pix_color_shift(TCAX_PyPix &PIX, int r, int g, int b, int a);

/**
 * Make the color in a PIX with specified rgb value transparent.
 * @param PIX
 * @param rgb
 * @return TCAX_PyPix
 */
//PixColorTrans(PIX, rgb)
extern TCAX_PyPix tcaxlib_pix_color_transparent(TCAX_PyPix &PIX, uint32_t rgb);

/**
 * Change the value of colors including alpha channels in the PIX with flat rgba value.
 * @param PIX
 * @param rgba
 * @return TCAX_PyPix
 */
//PixColorRGBA(PIX, rgba)
extern TCAX_PyPix tcaxlib_pix_color_flat_rgba(TCAX_PyPix &PIX, uint32_t rgba);

/**
 * Change the value of colors in the PIX with flat rgb value.
 * @param PIX
 * @param rgb
 * @return TCAX_PyPix
 */
//PixColorRGB(PIX, rgb)
extern TCAX_PyPix tcaxlib_pix_color_flat_rgb(TCAX_PyPix &PIX, uint32_t rgb);

/**
 * Change the value of alpha channels in the PIX with constant alpha value.
 * @param PIX
 * @param alpha
 * @return TCAX_PyPix
 */
//PixColorA(PIX, alpha)
extern TCAX_PyPix tcaxlib_pix_color_flat_alpha(TCAX_PyPix &PIX, unsigned int alpha);

/**
 * Bilinear filter, mainly used in moving a PIX, so that it will look smoother. 
 * Remark: this function is for internal use, it is used to manipulate a TCAX PIX.
 *
 * @see tcaxlib_bilinear_filter()
 * @param pPixSrc pointer to the source TCAX_Pix structure
 * @param offset_x offset of the PIX in the horizontal direction
 * @param offset_y offset of the PIX in the vertical direction
 * @param pPixDst pointer to the target TCAX_Pix structure
 * @return const TCAX_pPix
 */
extern TCAX_pPix tcaxlib_bilinear_filter_internal(const TCAX_pPix pPixSrc, double offset_x, double offset_y, TCAX_pPix pPixDst);

/**
 * Bilinear filter, mainly used in moving a PIX, so that it will look smoother.
 * @see tcaxlib_bilinear_filter_internal()
 * @param PIX
 * @param offset_x
 * @param offset_y
 * @return TCAX_PyPix
 */
//BilinearFilter(PIX, offset_x, offset_y)
extern TCAX_PyPix tcaxlib_bilinear_filter(TCAX_PyPix &PIX, double offset_x, double offset_y);

/**
 * Scale filter, mainly used in moving a PIX, so that it will look smoother. 
 * Remark: this function is for internal use, it is used to manipulate a TCAX PIX.
 * The difference between scale filter and bilinear filter is that, the scale filter is most fit for
 * complicated pictures such as text, rich colored pictures, while it is bad for simple solid color squares
 *
 * @see tcaxlib_scale_filter()
 * @param pPixSrc pointer to the source TCAX_Pix structure
 * @param offset_x offset of the PIX in the horizontal direction
 * @param offset_y offset of the PIX in the vertical direction
 * @param pPixDst pointer to the target TCAX_Pix structure
 * @return const TCAX_pPix
 */
extern TCAX_pPix tcaxlib_scale_filter_internal(const TCAX_pPix pPixSrc, double offset_x, double offset_y, TCAX_pPix pPixDst);

/**
 * Scale filter, mainly used in moving a PIX, so that it will look smoother.
 * @see tcaxlib_scale_filter_internal()
 * @param PIX
 * @param offset_x
 * @param offset_y
 * @return TCAX_PyPix
 */
//ScaleFilter(PIX, offset_x, offset_y)
extern TCAX_PyPix tcaxlib_scale_filter(TCAX_PyPix &PIX, double offset_x, double offset_y);

/**
 * Create a TCAX PY BIG PIX structure.
 * @param self reserved
 * @param args ()
 * @return TCAX_PyBigPix
 */
//InitBigPix()
extern TCAX_PyBigPix tcaxlib_init_big_pix();

/**
 * Add a PIX to BigPIX.
 * @param BIG_PIX
 * @param PIX
 * @param offset_x
 * @param offset_y
 * @param layer
 * @return py::long_
 */
//BigPixAdd(BIG_PIX, PIX, offset_x, offset_y, layer)
extern TCAX_Py_Error_Code tcaxlib_big_pix_add(TCAX_PyBigPix &BIG_PIX, TCAX_PyPix &PIX, double offset_x,
                                       double offset_y, int layer);

/**
 * Convert BigPIX to PIX.
 * @param BIG_PIX
 * @return TCAX_PyPix
 */
//ConvertBigPix(BIG_PIX)
extern TCAX_PyPix tcaxlib_convert_big_pix(TCAX_PyBigPix BIG_PIX);

#endif    /* TCAXLIB_PIX_H */
