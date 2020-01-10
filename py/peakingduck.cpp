////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

#include <fstream>
#include <functional>

#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include "common.hpp"
#include "peakingduck.hpp"

namespace py = pybind11;

NAMESPACE_USING(peakingduck)

PYBIND11_MODULE(pykingduck, m) {
    
    m.doc() = R"pbdoc(
        Peaking duck library using pybind11
        -------------------------------------------------

        ToDo: write

        A list:
            - Entry 1
            - Entry 2

        .. currentmodule:: pykingduck
        .. autosummary::
           :toctree: _generate

    )pbdoc"; // optional module docstring

    m.attr("__version__") = "0.0.1";

}