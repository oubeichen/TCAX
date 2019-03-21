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

#ifndef TCAXLIB_TEXT_H
#define TCAXLIB_TEXT_H
#pragma once

#include <string>
#include <cstdint>
#include <cstdlib>
#include "boost/locale.hpp"
#include "pix.h"
extern "C"
{
#include "freetype2/ft2build.h"
#include "freetype2/freetype/freetype.h"
#include "freetype2/freetype/ftstroke.h"
#include "freetype2/freetype/tttables.h"
#include "freetype2/freetype/ftimage.h"
#include "freetype2/freetype/fttypes.h"
};


#ifdef _WIN32
#pragma comment(lib, "../FreeType/freetype.lib")
#endif

#define FALSE 0
#define TRUE 1

#define TCAXLIB_TEXT_MAX(x, y) (x > y ? x : y)
#define TCAXLIB_TEXT_MIN(x, y) (x < y ? x : y)

/**
 * TCAX_Font structure
 */
typedef struct _tcax_font {
    const char *filename;   /**< font file name */
    FT_Library library;     /**< FT library */
    FT_Face face;           /**< FT face */
    int id;                 /**< font face id */
    int size;               /**< font size */
    int spacing;            /**< spacing between texts */
    double spaceScale;      /**< white space (including L' ' and L'　') scale */
    unsigned long color;    /**< color of the font */
    int bord;               /**< border of the font */
    int is_outline;         /**< indicates if the PIX only containing outline info */
} TCAX_Font, *TCAX_pFont;

/**
 * TCAX_OutlinePoints structure is used in tcaxlib_get_text_outline_as_points function().
 */
typedef struct _tcax_outline_points {
    TCAX_Points points;
    double lastX;
    double lastY;
    int height;
    double density;
} TCAX_OutlinePoints, *TCAX_pOutlinePoints;

/**
 * TCAX_TextMetrics structure
 */
typedef struct _tcax_text_metrics {
    /* FT_Glyph_Metrics */
    FT_Pos width;
    FT_Pos height;
    FT_Pos horiBearingX;
    FT_Pos horiBearingY;
    FT_Pos horiAdvance;
    FT_Pos vertBearingX;
    FT_Pos vertBearingY;
    FT_Pos vertAdvance;
    /* FT_Size_Metrics */
    FT_UShort x_ppem;   /* horizontal pixels per EM */
    FT_UShort y_ppem;   /* vertical pixels per EM */
    FT_Fixed x_scale;   /* scaling values used to convert font */
    FT_Fixed y_scale;   /* units to 26.6 fractional pixels */
    FT_Pos ascender;    /* ascender in 26.6 frac. pixels */
    FT_Pos descender;   /* descender in 26.6 frac. pixels */
    FT_Pos px_height;   /* text height in 26.6 frac. pixels */
    FT_Pos max_advance; /* max horizontal advance, in 26.6 pixels */
} TCAX_TextMetrics, *TCAX_pTextMetrics;

/**
 * TCAX_PyFont structure is only a python integer object to hold the value of pFont whose real content are kept in the 
 * memory, not passing to the python client, hence cannot be directly accessed from python scripts, but through tcaxLib APIs.
 */
typedef py::tuple TCAX_PyFont;

/**
 * TCAX_PyTextMetrics structure that has the following structure
 * (width, height, horiBearingX, horiBearingY, horiAdvance, vertBearingX, vertBearingY, vertAdvance, x_ppem, y_ppem, x_scale, y_scale, ascender, descender, px_height, max_advance)
 */
typedef py::tuple TCAX_PyTextMetrics;

/**
 * Initialize TCAX_Font structure
 */
extern int tcaxlib_init_font(TCAX_pFont pFont, const char *fontFilename, int fontFaceID,
                             int fontSize, int spacing, double spaceScale, uint32_t color,
                             int bord, int is_outline);

/**
 * Finalize TCAX_Font structure
 */
extern void tcaxlib_fin_font(TCAX_pFont pFont);

/**
 * Initialize TCAX_Font structure, keep the content in memory, then pass its pointer as TCAX_PyFont. 
 * Remark: the real content of TCAX_PyFont is kept in memory, and only can be accessed through tcaxLib APIs, 
 * the memory will be freed by FinFont function.
 *
 * @param font_file
 * @param face_id
 * @param font_size
 * @param spacing
 * @param space_scale
 * @param color
 * @param bord
 * @param is_outline
 * @return TCAX_PyFont
 */
//InitFont(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline)
extern TCAX_PyFont tcaxlib_init_py_font(const char *font_file, int face_id, int font_size, int spacing,
                                        double space_scale, uint32_t color, int bord, int is_outline);

/**
 * Finalize TCAX_Font structure free all the memory occupied by it.
 * @param pyFont
 * @return TCAX_Py_Error_Code
 */
//FinFont(pyFont)
extern TCAX_Py_Error_Code tcaxlib_fin_py_font(TCAX_PyFont &pyFont);

/**
 * Make TCAX PIX from text(s).
 * @param pyFont
 * @param texts
 * @return TCAX_PyPix
 */
//TextPix(pyFont, texts)
extern TCAX_PyPix tcaxlib_get_pix_from_text(TCAX_PyFont &pyFont, const char *texts);

/* overload */
//TextPix(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline, texts)
extern TCAX_PyPix tcaxlib_get_pix_from_text_2(const char *font_file, int face_id, int font_size, int spacing, double space_scale,
                                            uint32_t color, int bord, int is_outline, const char *texts);

/* Bezier Curve Evaluation using De Casteljau's Algorithm */
extern void linear_interpolation(double *pxt, double *pyt, double xa, double ya, double xb, double yb, double t);
extern void conic_bezier(double *pxt, double *pyt, double xs, double ys, double xc, double yc, double xe, double ye, double t);
extern void cubic_bezier(double *pxt, double *pyt, double xs, double ys, double xc1, double yc1, double xc2, double yc2, double xe, double ye, double t);

/**
 * Get text outline points.
 * @param pyFont
 * @param text
 * @param density
 * @return TCAX_PyPoints
 */
//TextOutlinePoints(pyFont, text, density)
extern TCAX_PyPoints tcaxlib_get_text_outline_as_points(TCAX_PyFont &pyFont, const char *text, double density);

/**
 * Get text outline points.
 * @param font_file
 * @param face_id
 * @param font_size
 * @param text
 * @param density
 * @return TCAX_PyPoints
 */
/* overload */
//TextOutlinePoints(font_file, face_id, font_size, text, density)
extern TCAX_PyPoints tcaxlib_get_text_outline_as_points_2(const char *font_file, int face_id, int font_size,
                                                          const char *text, double density);

/**
 * Get text(s) metrics.
 * @param self reserved
 * @param args (pyFont, texts) or (font_file, face_id, font_size, spacing, space_scale, text)
 * @return TCAX_PyTextMetrics
 */
//TextMetrics(pyFont, text)
extern TCAX_PyTextMetrics tcaxlib_get_text_metrics(TCAX_PyFont &pyFont, const char *text);

//TextMetrics(font_file, face_id, font_size, spacing, space_scale, text)
extern TCAX_PyTextMetrics tcaxlib_get_text_metrics_2(const char *font_file, int face_id, int font_size, int spacing,
                                                     double space_scale, const char *text);

#endif    /* TCAXLIB_TEXT_H */
