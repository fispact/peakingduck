////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

#include <fstream>
#include <functional>

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include "common.hpp"
#include "peakingduck.hpp"

namespace py = pybind11;

PEAKINGDUCK_NAMESPACE_USING(peakingduck)

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

    // version
    m.attr("__version__") = "0.0.1";

    // util module
    py::module m_util = m.def_submodule("util");

    // core module 
    py::module m_core = m.def_submodule("core");

    // io module
    py::module m_io = m.def_submodule("io");

    m_util.def("get_window", &util::get_window<double>,
            py::arg("values"),
            py::arg("centerindex"),
            py::arg("nouter") = 5,
            py::arg("ninner") = 0,
            py::arg("includeindex") = true);


    // core numerical object - tries to be like numpy array
    // but why you ask?
    // well this is a custom type specific for custom operations
    // like SNIP and others, that can be implemented in python
    // but we want it as quick as possible
    // can swap between numpy if need be
    using NumericalDataCoreType = double;
    using NumericalDataPyType = core::NumericalData<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<NumericalDataPyType>(m_core, "NumericalData")
        .def(py::init<>())
        .def(py::init<const std::vector<NumericalDataCoreType>&>())
        .def(py::init<const Eigen::Ref<core::Array1Dd>&>())
        .def(py::self + py::self)
        .def(py::self + NumericalDataCoreType())
        .def(py::self - py::self)
        .def(py::self - NumericalDataCoreType())
        .def(py::self * NumericalDataCoreType())
        .def(py::self / NumericalDataCoreType())
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= NumericalDataCoreType())
        .def(py::self /= NumericalDataCoreType())
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def(NumericalDataCoreType() + py::self)
        .def(NumericalDataCoreType() - py::self)
        .def(NumericalDataCoreType() * py::self)
        .def(NumericalDataCoreType() / py::self)
        .def(-py::self)
        .def("__call__",
             [](const NumericalDataPyType& data, int sindex, int eindex) {
                 return data(sindex, eindex);
             })
        .def("__len__",
             [](const NumericalDataPyType& data) {
                 return data.size();
             })
        .def("__getitem__",
             [](const NumericalDataPyType& data, size_t index) {
                 return data[index];
             })
        .def("from_list", &NumericalDataPyType::from_vector)
        .def("to_list", &NumericalDataPyType::to_vector)
        .def("LLS", &NumericalDataPyType::LLS)
        .def("LLSInPlace", &NumericalDataPyType::LLSInPlace)
        .def("inverseLLS", &NumericalDataPyType::inverseLLS)
        .def("inverseLLSInPlace", &NumericalDataPyType::inverseLLSInPlace)
        .def("midpoint", &NumericalDataPyType::midpoint)
        .def("midpointInPlace", &NumericalDataPyType::midpointInPlace)
        .def("snip", &NumericalDataPyType::snip,
            py::arg("niterations") = 20)
        .def("ramp", &NumericalDataPyType::ramp)
        .def("rampInPlace", &NumericalDataPyType::rampInPlace);

    // integral type
    using IntegerDataCoreType = int;
    using IntegerDataPyType = core::NumericalData<IntegerDataCoreType,core::ArrayTypeDynamic>;
    py::class_<IntegerDataPyType>(m_core, "IntegerData")
        .def(py::init<>())
        .def(py::init<const std::vector<IntegerDataCoreType>&>())
        .def(py::init<const Eigen::Ref<core::Array1Di>&>())
        .def(py::self + py::self)
        .def(py::self + IntegerDataCoreType())
        .def(py::self - py::self)
        .def(py::self - IntegerDataCoreType())
        .def(py::self * IntegerDataCoreType())
        .def(py::self / IntegerDataCoreType())
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= IntegerDataCoreType())
        .def(py::self /= IntegerDataCoreType())
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def(IntegerDataCoreType() + py::self)
        .def(IntegerDataCoreType() - py::self)
        .def(IntegerDataCoreType() * py::self)
        .def(IntegerDataCoreType() / py::self)
        .def(-py::self)
        .def("__call__",
             [](const IntegerDataPyType& data, int sindex, int eindex) {
                 return data(sindex, eindex);
             })
        .def("__len__",
             [](const IntegerDataPyType& data) {
                 return data.size();
             })
        .def("__getitem__",
             [](const IntegerDataPyType& data, size_t index) {
                 return data[index];
             })
        .def("from_list", &IntegerDataPyType::from_vector)
        .def("to_list", &IntegerDataPyType::to_vector)
        .def("ramp", &IntegerDataPyType::ramp)
        .def("rampInPlace", &IntegerDataPyType::rampInPlace);

    // smoothing objects
    using MovingAverageSmootherPyType = core::MovingAverageSmoother<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<MovingAverageSmootherPyType>(m_core, "MovingAverageSmoother")
        .def(py::init<int>())
        .def("__call__",
             [](const MovingAverageSmootherPyType& smoother, const NumericalDataPyType& data) {
                 return smoother(data);
             });

    using WeightedMovingAverageSmootherPyType = core::WeightedMovingAverageSmoother<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<WeightedMovingAverageSmootherPyType>(m_core, "WeightedMovingAverageSmoother")
        .def(py::init<int>())
        .def("__call__",
             [](const WeightedMovingAverageSmootherPyType& smoother, const NumericalDataPyType& data) {
                 return smoother(data);
             });

    // histogram objects
    using HistPyType = core::Histogram<double,double>;
    using HistChannelPyType = core::Histogram<int,double>;

    py::class_<HistPyType>(m_core, "Histogram")
        .def(py::init<>())
        .def(py::init<const core::NumericalData<double>&, const core::NumericalData<double>&>())
        .def(py::init<const HistPyType&>())
        .def_property_readonly("X", &HistPyType::X)
        .def_property_readonly("Y", &HistPyType::Y);

    py::class_<HistChannelPyType>(m_core, "HistogramChannelBased")
        .def(py::init<>())
        .def(py::init<const core::NumericalData<int>&, const core::NumericalData<double>&>())
        .def(py::init<const HistChannelPyType&>())
        .def_property_readonly("X", &HistChannelPyType::X)
        .def_property_readonly("Y", &HistChannelPyType::Y);

    // spectrum objects
    using SpectrumChannelBasedPyType = core::Spectrum<int,double>;
    py::class_<SpectrumChannelBasedPyType, HistChannelPyType>(m_core, "SpectrumChannelBased")
        .def(py::init<>())
        .def(py::init<const core::NumericalData<int>&, const core::NumericalData<double>&>())
        .def(py::init<const SpectrumChannelBasedPyType&>())
        .def("estimateBackground", &SpectrumChannelBasedPyType::estimateBackground)
        .def("removeBackground", &SpectrumChannelBasedPyType::removeBackground);

    using SpectrumEnergyBasedPyType = core::Spectrum<double,double>;
    py::class_<SpectrumEnergyBasedPyType, HistPyType>(m_core, "SpectrumEnergyBased")
        .def(py::init<>())
        .def(py::init<const core::NumericalData<double>&, const core::NumericalData<double>&>())
        .def(py::init<const SpectrumEnergyBasedPyType&>())
        .def("estimateBackground", &SpectrumEnergyBasedPyType::estimateBackground)
        .def("removeBackground", &SpectrumEnergyBasedPyType::removeBackground);

    // IO module read/write to file, etc...
    m_io.def("from_csv", 
            [](SpectrumEnergyBasedPyType& hist, const std::string& filename) {
                std::ifstream file(filename);
                //file >> m;
                core::io::Deserialize<double, double, ','>(file, hist);
                file.close();
            });
}