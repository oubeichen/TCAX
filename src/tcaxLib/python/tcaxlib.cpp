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

#include "boost/shared_ptr.hpp"
#include "../file.h"
#include "../pix.h"
#include "../text.h"
#include "../image.h"
#include "../utility.h"

const char *tcaxlib_get_version()
{
    return TCAXLIB_VER_STR;
}

/* overloads */
/* image */
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ImagePix_overloads, get_pix_from_image, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SavePix_overloads,  save_pix_to_image,  2, 4)

BOOST_PYTHON_MODULE(tcaxLib)
{
    /*      python_function_name cpp_function_name                                  actual_python_function */
    py::def("tcaxLibGetVersion", tcaxlib_get_version);                              //tcaxLibGetVersion()
    
    /* in pix.h */
    py::def("PixPoints",         tcaxlib_pix_points);                               //PixPoints(PIX)
    py::def("BlankPix",          tcaxlib_create_blank_pix);                         //BlankPix(width, height, rgba)
    py::def("PixResize",         tcaxlib_resample_py_pix);                          //PixResize(PIX, width, height)
    py::def("PixResizeF",        tcaxlib_resample_py_pix_ex);                       //PixResizeF(PIX, width, height)
    py::def("PixTexture",        tcaxlib_apply_pix_texture);                        //PixTexture(PIX, texture)
    py::def("PixMask",           tcaxlib_apply_pix_mask);                           //PixMask(PIX, mask)
    py::def("PixReplaceAlpha",   tcaxlib_pix_replace_alpha);                        //PixReplaceAlpha(PIX, PIX_alpha)
    py::def("PixEnlarge",        tcaxlib_enlarge_py_pix);                           //PixEnlarge(PIX, x, y)
    py::def("PixCrop",           tcaxlib_crop_py_pix);                              //PixCrop(PIX, left, top, right, bottom)
    py::def("PixStrip",          tcaxlib_py_pix_strip_blank);                       //PixStrip(PIX)
    py::def("PixBlur",           tcaxlib_apply_gauss_blur);                         //PixBlur(PIX, radius)
    py::def("CombinePixs",       tcaxlib_combine_two_pixs);                         //CombinePixs(back, overlay)
    py::def("PixColorMul",       tcaxlib_pix_color_multiply);                       //PixColorMul(PIX, r_f, g_f, b_f, a_f)
    py::def("PixColorShift",     tcaxlib_pix_color_shift);                          //PixColorShift(PIX, r, g, b, a)
    py::def("PixColorTrans",     tcaxlib_pix_color_transparent);                    //PixColorTrans(PIX, rgb)
    py::def("PixColorRGBA",      tcaxlib_pix_color_flat_rgba);                      //PixColorRGBA(PIX, rgba)
    py::def("PixColorRGB",       tcaxlib_pix_color_flat_rgb);                       //PixColorRGB(PIX, rgb)
    py::def("PixColorA",         tcaxlib_pix_color_flat_alpha);                     //PixColorA(PIX, alpha)
    py::def("BilinearFilter",    tcaxlib_bilinear_filter);                          //BilinearFilter(PIX, offset_x, offset_y)
    py::def("ScaleFilter",       tcaxlib_scale_filter);                             //ScaleFilter(PIX, offset_x, offset_y)
    py::def("InitBigPix",        tcaxlib_init_big_pix);                             //InitBigPix()
    py::def("BigPixAdd",         tcaxlib_big_pix_add);                              //BigPixAdd(BIG_PIX, PIX, offset_x, offset_y, layer)
    py::def("ConvertBigPix",     tcaxlib_convert_big_pix);                          //ConvertBigPix(BIG_PIX)
    py::register_ptr_to_python<TCAX_pPix>();
    py::register_ptr_to_python<TCAX_pPoints>();

    /* in file.h */
    py::class_<file>("file", py::init<const char *, py::optional<const char *>>())
    .def("WriteAssFile", &file::write_ass_file)
    .def("reset", &file::reset)
    .def("isSuccess", &file::is_success)
    .def("isAppend", &file::is_append)
    ;

    /* in text.h */
    py::def("InitFont",          tcaxlib_init_py_font);                             //InitFont(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline)
    py::def("FinFont",           tcaxlib_fin_py_font);                              //FinFont(pyFont)
    py::def("TextPix",           tcaxlib_get_pix_from_text);                        //TextPix(pyFont, texts)
    py::def("TextPix",           tcaxlib_get_pix_from_text_2);                      //TextPix(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline, texts)
    py::def("TextOutlinePoints", tcaxlib_get_text_outline_as_points);               //TextOutlinePoints(pyFont, text, density)
    py::def("TextOutlinePoints", tcaxlib_get_text_outline_as_points_2);             //TextOutlinePoints(font_file, face_id, font_size, text, density)
    py::def("TextMetrics",       tcaxlib_get_text_metrics);                         //TextMetrics(pyFont, text)
    py::def("TextMetrics",       tcaxlib_get_text_metrics_2);                       //TextMetrics(font_file, face_id, font_size, spacing, space_scale, text)

    /* in image.h */
    py::class_<image>("image")
    .def("ImagePix",             &image::get_pix_from_image, ImagePix_overloads()) //ImagePix(filename, width = 0, height = 0)
    .def("SavePix",              &image::save_pix_to_image, SavePix_overloads()) //SavePix(filename, PIX, width = 0, height = 0)
    ;

    /* in utility.h */
    py::def("TextOutlineDraw",   tcaxlib_get_text_outline_as_string);               //TextOutlineDraw(pyFont, text, x, y)
    py::def("TextOutlineDraw",   tcaxlib_get_text_outline_as_string_2);             //TextOutlineDraw(font_file, face_id, font_size, text, x, y)
    py::def("IsCjk",             tcaxlib_is_c_or_j_or_k);                           //IsCjk(text)
    py::def("VertLayout",        tcaxlib_vertical_layout_ass);                      //VertLayout(text)
    py::def("ShowProgress",      tcaxlib_show_progress);                            //ShowProgress(total, completed, file_id, file_num)
    py::def("Bezier1",           tcaxlib_bezier_curve_linear);                      //Bezier1(nPoints, xs, ys, xe, ye)
    py::def("Bezier2",           tcaxlib_bezier_curve_quadratic);                   //Bezier2(nPoints, xs, ys, xe, ye, xc, yc)
    py::def("Bezier3",           tcaxlib_bezier_curve_cubic);                       //Bezier3(nPoints, xs, ys, xe, ye, xc1, yc1, xc2, yc2)
    py::def("BezierN",           tcaxlib_bezier_curve_random);                      //BezierN(nPoints, xs, ys, xe, ye, xl1, yl1, xl2, yl2, order)
    py::def("GetFontSize",       tcaxlib_get_actual_font_size);                     //GetFontSize(font_file, face_id, font_size)
    py::def("CairoFontSize",     tcaxlib_calc_cairo_font_size);                     //CairoFontSize(font_file, face_id, font_size)
    py::register_ptr_to_python<TCAX_pString>();
    py::register_ptr_to_python<TCAX_pOutlineString>();
}
