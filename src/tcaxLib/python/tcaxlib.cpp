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

#include "../file.h"
#include "../pix.h"
#include "../text.h"
//#include "image.h"
//#include "utility.h"
//#include "tcasfunc.h"

const char *tcaxlib_get_version()
{
    return TCAXLIB_VER_STR;
}

BOOST_PYTHON_MODULE(tcaxLib)
{
    /*      python_function_name cpp_function_name               actual_python_function*/
    py::def("tcaxLibGetVersion", tcaxlib_get_version);           //tcaxLibGetVersion()
    
    /* in pix.h */
    py::def("PixPoints",         tcaxlib_pix_points);            //PixPoints(PIX)
    py::def("BlankPix",          tcaxlib_create_blank_pix);      //BlankPix(width, height, rgba)
    py::def("PixResize",         tcaxlib_resample_py_pix);       //PixResize(PIX, width, height)
    py::def("PixResizeF",        tcaxlib_resample_py_pix_ex);    //PixResizeF(PIX, width, height)
    py::def("PixTexture",        tcaxlib_apply_pix_texture);     //PixTexture(PIX, texture)
    py::def("PixMask",           tcaxlib_apply_pix_mask);        //PixMask(PIX, mask)
    py::def("PixReplaceAlpha",   tcaxlib_pix_replace_alpha);     //PixReplaceAlpha(PIX, PIX_alpha)
    py::def("PixEnlarge",        tcaxlib_enlarge_py_pix);        //PixEnlarge(PIX, x, y)
    py::def("PixCrop",           tcaxlib_crop_py_pix);           //PixCrop(PIX, left, top, right, bottom)
    py::def("PixStrip",          tcaxlib_py_pix_strip_blank);    //PixStrip(PIX)
    py::def("PixBlur",           tcaxlib_apply_gauss_blur);      //PixBlur(PIX, radius)
    py::def("CombinePixs",       tcaxlib_combine_two_pixs);      //CombinePixs(back, overlay)
    py::def("PixColorMul",       tcaxlib_pix_color_multiply);    //PixColorMul(PIX, r_f, g_f, b_f, a_f)
    py::def("PixColorShift",     tcaxlib_pix_color_shift);       //PixColorShift(PIX, r, g, b, a)
    py::def("PixColorTrans",     tcaxlib_pix_color_transparent); //PixColorTrans(PIX, rgb)
    py::def("PixColorRGBA",      tcaxlib_pix_color_flat_rgba);   //PixColorRGBA(PIX, rgba)
    py::def("PixColorRGB",       tcaxlib_pix_color_flat_rgb);    //PixColorRGB(PIX, rgb)
    py::def("PixColorA",         tcaxlib_pix_color_flat_alpha);  //PixColorA(PIX, alpha)
    py::def("BilinearFilter",    tcaxlib_bilinear_filter);       //BilinearFilter(PIX, offset_x, offset_y)
    py::def("ScaleFilter",       tcaxlib_scale_filter);          //ScaleFilter(PIX, offset_x, offset_y)
    py::def("InitBigPix",        tcaxlib_init_big_pix);          //InitBigPix()
    py::def("BigPixAdd",         tcaxlib_big_pix_add);           //BigPixAdd(BIG_PIX, PIX, offset_x, offset_y, layer)
    py::def("ConvertBigPix",     tcaxlib_convert_big_pix);       //ConvertBigPix(BIG_PIX)
    
    /* in file.h */
    py::def("CreateAssFile",     tcaxlib_create_ass_file);       //CreateAssFile(ass_file, ass_header)
    py::def("AppendAssFile",     tcaxlib_append_ass_file);       //AppendAssFile(ass_file)
    py::def("WriteAssFile",      tcaxlib_write_ass_file);        //WriteAssFile(pyAssFile, ASS_BUF)
    py::def("FinAssFile",        tcaxlib_fin_ass_file);          //FinAssFile(pyAssFile)

    /* in text.h */
    py::def("InitFont", tcaxlib_init_py_font); //InitFont(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline)
    py::def("FinFont", tcaxlib_fin_py_font); //FinFont(pyFont)
    py::def("TextPix", tcaxlib_get_pix_from_text); //TextPix(pyFont, texts)
    py::def("TextPix", tcaxlib_get_pix_from_text_2);//TextPix(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline, texts)
    py::def("TextOutlinePoints", tcaxlib_get_text_outline_as_points); //TextOutlinePoints(pyFont, text, density)
    py::def("TextOutlinePoints", tcaxlib_get_text_outline_as_points_2); //TextOutlinePoints(font_file, face_id, font_size, text, density)
    py::def("TextMetrics", tcaxlib_get_text_metrics); //TextMetrics(pyFont, text)
    py::def("TextMetrics", tcaxlib_get_text_metrics_2); //TextMetrics(font_file, face_id, font_size, spacing, space_scale, text)
}

/*
static PyMethodDef tcaxLib_Methods[] = {
    in image.h
    { "ImagePix"          ,  tcaxlib_get_pix_from_image              ,  METH_VARARGS ,  "ImagePix(filename) or ImagePix(filename, width, height)" },
    { "SavePix"           ,  tcaxlib_save_pix_to_image               ,  METH_VARARGS ,  "SavePix(filename, PIX) or SavePix(filename, PIX, width, height)" },

    in utility.h
    { "TextOutlineDraw"   ,  tcaxlib_get_text_outline_as_string      ,  METH_VARARGS ,  "TextOutlineDraw(pyFont, text, x, y) or TextOutlineDraw(font_file, face_id, font_size, text, x, y)" },
    { "IsCjk"             ,  tcaxlib_is_c_or_j_or_k                  ,  METH_VARARGS ,  "IsCjk(text)" },
    { "VertLayout"        ,  tcaxlib_vertical_layout_ass             ,  METH_VARARGS ,  "VertLayout(text)" },
    { "ShowProgress"      ,  tcaxlib_show_progress                   ,  METH_VARARGS ,  "ShowProgress(total, completed, file_id, file_num)" },
    { "Bezier1"           ,  tcaxlib_bezier_curve_linear             ,  METH_VARARGS ,  "Bezier1(nPoints, xs, ys, xe, ye)" },
    { "Bezier2"           ,  tcaxlib_bezier_curve_quadratic          ,  METH_VARARGS ,  "Bezier2(nPoints, xs, ys, xe, ye, xc, yc)" },
    { "Bezier3"           ,  tcaxlib_bezier_curve_cubic              ,  METH_VARARGS ,  "Bezier3(nPoints, xs, ys, xe, ye, xc1, yc1, xc2, yc2)" },
    { "BezierN"           ,  tcaxlib_bezier_curve_random             ,  METH_VARARGS ,  "BezierN(nPoints, xs, ys, xe, ye, xl1, yl1, xl2, yl2, order)" },
    { "GetFontSize"       ,  tcaxlib_get_actual_font_size            ,  METH_VARARGS ,  "GetFontSize(font_file, face_id, font_size)" },
    { "CairoFontSize"     ,  tcaxlib_calc_cairo_font_size            ,  METH_VARARGS ,  "CairoFontSize(font_file, face_id, font_size)" },

    in tcasfunc.h
    { "tcas_main"         ,  tcaxlib_tcas_list_append_pix            ,  METH_VARARGS ,  "tcas_main(TCAS_BUF, pix, start, end, offsetX, offsetY, layer)" },
    { "tcas_keyframe"     ,  tcaxlib_tcas_list_append_key_pixs       ,  METH_VARARGS ,  "tcas_keyframe(TCAS_BUF, pix_start, pix_end, start, end, offsetX, offsetY, type, layer)" },
    { "tcas_parse"        ,  tcaxlib_tcas_list_parse                 ,  METH_VARARGS ,  "tcas_parse(TCAS_BUF, width, height, fps, layer)" },
    { NULL                ,  NULL                                    ,  0            ,  NULL }
};

*/
