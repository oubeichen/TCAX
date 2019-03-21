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

#include "file.h"

//CreateAssFile(ass_file, ass_header)
TCAX_PyAssFile tcaxlib_create_ass_file(const char *ass_file, const char *ass_header)
{
    TCAX_pAssFile pAssFile;
    TCAX_PyAssFile pyAssFile;
    
    pAssFile = (TCAX_pAssFile)malloc(sizeof(TCAX_AssFile));
    pAssFile->fp = fopen(ass_file, "wb");
    if (!pAssFile->fp) {
        free(pAssFile);
        PyErr_SetString(PyExc_RuntimeError, "CreateAssFile error, failed to create the file!\n");
        return py::tuple();
    }
    
    fwrite(ass_header, sizeof(char), strlen(ass_header), pAssFile->fp);
    pyAssFile = py::make_tuple(pAssFile);
    return pyAssFile;
}

int _tcaxlib_check_if_file_exists(const char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp)
    {
        fclose(fp);
        return 1;
    }
    
    return 0;
}

//AppendAssFile(ass_file)
TCAX_PyAssFile tcaxlib_append_ass_file(const char *ass_file)
{
    TCAX_pAssFile pAssFile;
    TCAX_PyAssFile pyAssFile;
    
    if (!_tcaxlib_check_if_file_exists(ass_file))
    {
        PyErr_SetString(PyExc_RuntimeError, "AppendAssFile error, failed to open the file!\n");
        return py::tuple();
    }
    
    pAssFile = (TCAX_pAssFile)malloc(sizeof(TCAX_AssFile));
    pAssFile->fp = fopen(ass_file, "ab");
    if (!pAssFile->fp) {
        free(pAssFile);
        PyErr_SetString(PyExc_RuntimeError, "AppendAssFile error, failed to open the file!\n");
        return py::tuple();
    }
    
    pyAssFile = py::make_tuple(pAssFile);
    return pyAssFile;
}

static void _tcaxlib_convert_ass_list_to_string(py::list &assList, char **pAssString, int *pCount)
{
    int i, count, size, offset;
    char *assString;
    char **pAssLine;
    int *assLineSize;
    count = static_cast<int>(py::len(assList));
    assLineSize = (int *)malloc(count * sizeof(int));
    pAssLine = (char **)malloc(count * sizeof(char *));
    size = 0;
    for (i = 0; i < count; i++)
    {
        pAssLine[i] = py::extract<char *>(assList[i]);
        assLineSize[i] = static_cast<int>(strlen(pAssLine[i]));
        size += assLineSize[i];
    }
    
    assString = (char *)malloc((size + 1) * sizeof(char));
    offset = 0;
    for (i = 0; i < count; i++) {
        memcpy(assString + offset, pAssLine[i], assLineSize[i] * sizeof(char));
        offset += assLineSize[i];
    }
    assString[size] = '\0';
    free(pAssLine);
    free(assLineSize);
    *pAssString = assString;
    *pCount = size;
}

//WriteAssFile(pyAssFile, ASS_BUF)
TCAX_Py_Error_Code tcaxlib_write_ass_file(TCAX_PyAssFile &pyAssFile, py::list &ASS_BUF)
{
    TCAX_pAssFile pAssFile;
    char *assString;
    int size;
    
    if (py::len(ASS_BUF) == 0)
    {
        PyErr_SetString(PyExc_RuntimeWarning, "WriteAssFile warning, empty list 'ASS_BUF'\n");
        return py::long_(-1);
    }
    
    pAssFile = py::extract<TCAX_pAssFile>(pyAssFile[0]);
    _tcaxlib_convert_ass_list_to_string(ASS_BUF, &assString, &size);
    fwrite(assString, sizeof(char), size, pAssFile->fp);
    free(assString);
    return py::long_(0);
}

//FinAssFile(pyAssFile)
TCAX_Py_Error_Code tcaxlib_fin_ass_file(TCAX_PyAssFile &pyAssFile)
{
    TCAX_pAssFile pAssFile;
    
    pAssFile = py::extract<TCAX_pAssFile>(pyAssFile[0]);
    fclose(pAssFile->fp);
    free(pAssFile);
    pyAssFile = py::tuple(); //reset to none
    return py::long_(0);
}
