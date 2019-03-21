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

#ifndef TCAXLIB_FILE_H
#define TCAXLIB_FILE_H
#pragma once

#include <cstring>

#include "std.h"
//#include "tcasfunc.h"

/**
 * TCAX_AssFile structure
 */
typedef struct _tcax_ass_file {
    FILE *fp;
} TCAX_AssFile, *TCAX_pAssFile;

/**
 * TCAX_PyAssFile structure that has the following form (TCAX_pAssFile)
 */
typedef py::tuple TCAX_PyAssFile;

/**
 * TCAX_PyTcasFile structure that has the following form (TCAX_pTcasFile)
 */
typedef py::tuple TCAX_PyTcasFile;

/**
 * Create an ASS file and returns the handler to the file.
 * @param ass_file
 * @param ass_header
 * @return TCAX_PyAssFile;
 */
//CreateAssFile(ass_file, ass_header)
extern TCAX_PyAssFile tcaxlib_create_ass_file(const char *ass_file, const char *ass_header);

/**
 * Append to an existing ASS file and returns the handler to the file.
 * @param ass_file
 * @return TCAX_PyAssFile;
 */
//AppendAssFile(ass_file)
extern TCAX_PyAssFile tcaxlib_append_ass_file(const char *ass_file);

/**
 * Write ASS strings to ASS file.
 * @param pyAssFile
 * @param ASS_BUF
 * @return TCAX_Py_Error_Code;
 */
//WriteAssFile(pyAssFile, ASS_BUF)
extern TCAX_Py_Error_Code tcaxlib_write_ass_file(TCAX_PyAssFile &pyAssFile, py::list &ASS_BUF);

/**
 * Finalize the ASS file (close the handler to the file and destroy contents assigned to it).
 * @param pyAssFile
 * @return TCAX_Py_Error_Code;
 */
//FinAssFile(pyAssFile)
extern TCAX_Py_Error_Code tcaxlib_fin_ass_file(TCAX_PyAssFile &pyAssFile);

#endif    /* TCAXLIB_FILE_H */
