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

#ifndef TCAXLIB_COMMON_H
#define TCAXLIB_COMMON_H

#pragma once

#include <algorithm>
#include <cstdint>
#include "boost/python.hpp"
#include "python3.7m/Python.h"

namespace tcaxLib
{
    namespace py = boost::python;

    typedef py::long_ TCAX_Py_Error_Code;
};

#endif //TCAXLIB_COMMON_H