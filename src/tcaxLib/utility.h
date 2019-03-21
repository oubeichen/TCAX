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

#ifndef TCAXLIB_UTILITY_H
#define TCAXLIB_UTILITY_H
#pragma once

#include "pix.h"
#include "text.h"


/**
 * TCAX_String structure is an auto length increase string.
 */
typedef struct _tcax_string {
    wchar_t *buf;
    int len;
    int capacity;
} TCAX_String, *TCAX_pString;

/**
 * TCAX_OutlineState structure is used in tcaxlib_get_text_outline_as_string function().
 */
typedef struct _tcax_outline_string {
    TCAX_String string;    /**< string that contains the drawing commands */
    FT_Vector last;        /**< last point position */
    FT_Pos height;         /**< virtual height of the text, not the real height, used to convert the orientation */
    FT_Vector offset;      /**< translate the position */
} TCAX_OutlineString, *TCAX_pOutlineString;

/**
 * TCAX_PyString structure that is in fact a Python unicode object.
 */
typedef py::str TCAX_PyString;

/**
 * TCAX_PyLong structure that is in fact a Python integer object.
 */
typedef py::long_ TCAX_PyLong;

/**
 * TCAX_PyFloat structure that is in fact a Python float object.
 */
typedef py::tuple TCAX_PyFloat;
    
/**
 * Get the text outline (which is consisted of contours, lines, bezier arcs) as a string of ASS drawing commands.
 * @param pyFont
 * @param text
 * @param x
 * @param y
 * @return TCAX_PyString
 */
//TextOutlineDraw(pyFont, text, x, y)
extern TCAX_PyString tcaxlib_get_text_outline_as_string(TCAX_PyFont &pyFont, const char *text, int x, int y);

/**
 * Get the text outline (which is consisted of contours, lines, bezier arcs) as a string of ASS drawing commands.
 * @param font_file
 * @param face_id
 * @param font_size
 * @param text
 * @param x
 * @param y
 * @return TCAX_PyString
 */
//TextOutlineDraw(font_file, face_id, font_size, text, x, y)
extern TCAX_PyString tcaxlib_get_text_outline_as_string_2(const char *font_file, int face_id, int font_size,
                                                          const char *text, int x, int y);

/**
 * Check if a text string is a CJK string. 
 * Remark: here CJK means "Chinese or Japanese or Korea" not "Chinese & Japanese & Korea".
 * @param text
 * @return TCAX_Py_Error_Code
 */
//IsCjk(text)
extern TCAX_Py_Error_Code tcaxlib_is_c_or_j_or_k(const char *text);

/**
 * Change to vertical text layout.
 * @param text
 * @return TCAX_PyString
 */
//VertLayout(text)
extern TCAX_PyString tcaxlib_vertical_layout_ass(const char *text);

/**
 * Show progress.
 * @param total
 * @param completed
 * @param file_id
 * @param file_num
 * @return TCAX_Py_Error_Code
 */
//ShowProgress(total, completed, file_id, file_num)
extern TCAX_Py_Error_Code tcaxlib_show_progress(int total, int completed, int file_id, int file_num);

/**
 * Linear Bezier Curve.
 * @param points
 * @param xs
 * @param ys
 * @param xe
 * @param ye
 * @return TCAX_PyPoints
 */
//Bezier1(nPoints, xs, ys, xe, ye)
extern TCAX_PyPoints tcaxlib_bezier_curve_linear(int points,
                                                 double xs, double ys,
                                                 double xe, double ye);

/**
 * Quadratic Bezier Curve.
 * @param points
 * @param xs
 * @param ys
 * @param xe
 * @param ye
 * @param xc
 * @param yc
 * @return TCAX_PyPoints
 */
//Bezier2(nPoints, xs, ys, xe, ye, xc, yc)
extern TCAX_PyPoints tcaxlib_bezier_curve_quadratic(int points,
                                                    double xs, double ys,
                                                    double xe, double ye,
                                                    double xc, double yc);

/**
 * Cubic Bezier Curve.
 * @param points
 * @param xs
 * @param ys
 * @param xe
 * @param ye
 * @param xc1
 * @param yc1
 * @param xc2
 * @param yc2
 * @return TCAX_PyPoints
 */
//Bezier3(nPoints, xs, ys, xe, ye, xc1, yc1, xc2, yc2)
extern TCAX_PyPoints tcaxlib_bezier_curve_cubic(int points,
                                                double xs, double ys,
                                                double xe, double ye,
                                                double xc1, double yc1,
                                                double xc2, double yc2);

/**
 * Nth Order Bezier Curve with random control points.
 * @param points
 * @param xs
 * @param ys
 * @param xe
 * @param ye
 * @param xl1
 * @param yl1
 * @param xl2
 * @param yl2
 * @param order
 * @return TCAX_PyPoints
 */
//BezierN(nPoints, xs, ys, xe, ye, xl1, yl1, xl2, yl2, order)
extern TCAX_PyPoints tcaxlib_bezier_curve_random(int points,
                                                 double xs, double ys,
                                                 double xe, double ye,
                                                 double xl1, double yl1,
                                                 double xl2, double yl2,
                                                 int order);

/**
 * Calculate the actual font size used in tcaxLib and util.magick modules.
 * @param self reserved
 * @param args (font_file, face_id, font_size)
 * @return TCAX_PyFloat
 */
//GetFontSize(font_file, face_id, font_size)
extern TCAX_PyFloat tcaxlib_get_actual_font_size(const char *font_file, int face_id, int font_size);

/**
 * Calculate a correct font size for cairo from GDI font size.
 * @param font_file
 * @param face_id
 * @param font_size
 * @return TCAX_PyLong
 */
//CairoFontSize(font_file, face_id, font_size)
extern TCAX_PyLong tcaxlib_calc_cairo_font_size(const char *font_file, int face_id, int font_size);

#endif    /* TCAXLIB_UTILITY_H */
