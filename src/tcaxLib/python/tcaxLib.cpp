#include "../common.h"
#include "../pix.h"

using namespace tcaxLib;

BOOST_PYTHON_MODULE(tcaxLib)
{
    py::class_<pix>("pix")
            .def("pix_points", &pix::pix_points)
            .def("BlankPix", &pix::create_blank_pix)
            .def("PixResize", &pix::resample_py_pix)
            .def("PixResizeF", &pix::resample_py_pix_ex)
    ;
}