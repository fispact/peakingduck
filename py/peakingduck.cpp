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

PYBIND11_MODULE(PEAKINGDUCK, m) {
    
    m.doc() = R"pbdoc(
        Peaking duck library using pybind11
        -------------------------------------------------

        ToDo: write

        A list:
            - Entry 1
            - Entry 2

        .. currentmodule:: peakingduck
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
        .def("__iter__", 
             [](const NumericalDataPyType &data) { 
                return py::make_iterator(data.begin(), data.end()); 
             }, py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__", 
             [](const NumericalDataPyType& data) {
                 return data.size();
             })
        .def("__getitem__",
             [](const NumericalDataPyType& data, size_t index) {
                 return data[index];
             })
        .def("__setitem__",
             [](NumericalDataPyType& data, size_t index, NumericalDataCoreType value) {
                data[index] = value;
             })
        .def("from_list", &NumericalDataPyType::from_vector)
        .def("to_list", &NumericalDataPyType::to_vector)
        .def("LLS", &NumericalDataPyType::LLS)
        .def("LLSInPlace", &NumericalDataPyType::LLSInPlace)
        .def("inverseLLS", &NumericalDataPyType::inverseLLS)
        .def("inverseLLSInPlace", &NumericalDataPyType::inverseLLSInPlace)
        .def("midpoint", &NumericalDataPyType::midpoint)
        .def("midpointInPlace", &NumericalDataPyType::midpointInPlace)
        .def("snip", [](const NumericalDataPyType& data, const std::vector<int>& iteration_list){
            return data.snip(iteration_list.begin(), iteration_list.end());
        })
        .def("snip", (NumericalDataPyType (NumericalDataPyType::*)(int) const)&NumericalDataPyType::snip,
            py::arg("niterations") = 20)

        /* pybind cannot deduce types so we need to use old style function pointers */
        // .def("snip", py::overload_cast<std::vector<int>::iterator,std::vector<int>::iterator>(&NumericalDataPyType::snip))
        // .def("snip", py::overload_cast<int>(&NumericalDataPyType::snip),
        //     py::arg("niterations") = 20)

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
        .def("__iter__", 
             [](const IntegerDataPyType &data) { 
                return py::make_iterator(data.begin(), data.end()); 
             }, py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__", 
             [](const IntegerDataPyType& data) {
                 return data.size();
             })
        .def("__getitem__",
             [](const IntegerDataPyType& data, size_t index) {
                 return data[index];
             })
        .def("__setitem__",
             [](IntegerDataPyType& data, size_t index, IntegerDataCoreType value) {
                data[index] = value;
             })
        .def("from_list", &IntegerDataPyType::from_vector)
        .def("to_list", &IntegerDataPyType::to_vector)
        .def("ramp", &IntegerDataPyType::ramp)
        .def("rampInPlace", &IntegerDataPyType::rampInPlace);

    // core process object
    using IProcessPyType = core::IProcess<NumericalDataCoreType,core::ArrayTypeDynamic>;

    class PyProcess : public IProcessPyType {
        public:
            /* Inherit the constructors */
            using IProcessPyType::IProcess;

            /* Trampoline (need one for each virtual function) */
            NumericalDataPyType
            go(const NumericalDataPyType& data) const override {
                PYBIND11_OVERLOAD_PURE(
                    NumericalDataPyType,    /* Return type */
                    IProcessPyType,         /* Parent class */
                    go,                     /* Name of function in C++ (must match Python name) */
                    data                    /* Argument(s) */
                );
            }
    };

    py::class_<IProcessPyType, PyProcess, std::shared_ptr<IProcessPyType>>(m_core, "IProcess")
        .def(py::init_alias<>())
        .def("go", &IProcessPyType::go);

    // process manager
    using IProcessManagerPyType = core::IProcessManager<NumericalDataCoreType,core::ArrayTypeDynamic>;

    class PyProcessManager : public IProcessManagerPyType {
        public:
            /* Inherit the constructors */
            using IProcessManagerPyType::IProcessManager;

            /* Trampoline (need one for each virtual function) */
            IProcessManager&
            append(const std::shared_ptr<IProcessPyType>& process) override {
                PYBIND11_OVERLOAD_PURE(
                    IProcessManager&,   /* Return type */
                    IProcessManager,    /* Parent class */
                    append,             /* Name of function in C++ (must match Python name) */
                    process             /* Argument(s) */
                );
            }

            NumericalDataPyType
            run(const NumericalDataPyType& data) const override {
                PYBIND11_OVERLOAD_PURE(
                    NumericalDataPyType,    /* Return type */
                    IProcessManager,        /* Parent class */
                    run,                    /* Name of function in C++ (must match Python name) */
                    data                    /* Argument(s) */
                );
            }

            size_t
            size() const override {
                PYBIND11_OVERLOAD_PURE(
                    size_t,                 /* Return type */
                    IProcessManager,        /* Parent class */
                    size                    /* Name of function in C++ (must match Python name) */
                );
            }

            void
            reset() override {
                PYBIND11_OVERLOAD_PURE(
                    void,                   /* Return type */
                    IProcessManager,        /* Parent class */
                    reset                   /* Name of function in C++ (must match Python name) */
                );
            }
    };

    py::class_<IProcessManagerPyType, PyProcessManager, std::shared_ptr<IProcessManagerPyType>>(m_core, "IProcessManager")
        .def(py::init_alias<>())
        .def("append", &IProcessManagerPyType::append)
        .def("run", &IProcessManagerPyType::run)
        .def("__len__", &IProcessManagerPyType::size)
        .def("reset", &IProcessManagerPyType::reset);

    // simple process manager
    using SimpleProcessManagerPyType = core::SimpleProcessManager<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<SimpleProcessManagerPyType, IProcessManagerPyType, std::shared_ptr<SimpleProcessManagerPyType>>(m_core, "SimpleProcessManager")
        .def(py::init<>())
        .def("append", &SimpleProcessManagerPyType::append)
        .def("run", &SimpleProcessManagerPyType::run)
        .def("__len__", &SimpleProcessManagerPyType::size)
        .def("reset", &SimpleProcessManagerPyType::reset);

    // smoothing objects
    using MovingAverageSmootherPyType = core::MovingAverageSmoother<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<MovingAverageSmootherPyType, IProcessPyType, std::shared_ptr<MovingAverageSmootherPyType>>(m_core, "MovingAverageSmoother")
        .def(py::init<int>());

    using WeightedMovingAverageSmootherPyType = core::WeightedMovingAverageSmoother<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<WeightedMovingAverageSmootherPyType, IProcessPyType, std::shared_ptr<WeightedMovingAverageSmootherPyType>>(m_core, "WeightedMovingAverageSmoother")
        .def(py::init<int>());

    // peak finding objects
    using GlobalThresholdPeakFinderPyType = core::GlobalThresholdPeakFinder<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<GlobalThresholdPeakFinderPyType, IProcessPyType, std::shared_ptr<GlobalThresholdPeakFinderPyType>>(m_core, "GlobalThresholdPeakFinder")
        .def(py::init<NumericalDataCoreType>());

    using ChunkedThresholdPeakFinderPyType = core::ChunkedThresholdPeakFinder<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<ChunkedThresholdPeakFinderPyType, IProcessPyType, std::shared_ptr<ChunkedThresholdPeakFinderPyType>>(m_core, "ChunkedThresholdPeakFinder")
        .def(py::init<NumericalDataCoreType, size_t>(), 
            py::arg("percentThreshold"), 
            py::arg("chunkSize") = 10);

    using MovingAveragePeakFinderPyType = core::MovingAveragePeakFinder<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<MovingAveragePeakFinderPyType, IProcessPyType, std::shared_ptr<MovingAveragePeakFinderPyType>>(m_core, "MovingAveragePeakFinder")
        .def(py::init<int>());

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
        .def("estimateBackground", [](const SpectrumChannelBasedPyType& spectrum, const std::vector<int>& iteration_list){
            return spectrum.estimateBackground(iteration_list.begin(), iteration_list.end());
        })
        .def("removeBackground", [](SpectrumChannelBasedPyType& spectrum, const std::vector<int>& iteration_list){
            spectrum.removeBackground(iteration_list.begin(), iteration_list.end());
        });

    using SpectrumEnergyBasedPyType = core::Spectrum<double,double>;
    py::class_<SpectrumEnergyBasedPyType, HistPyType>(m_core, "SpectrumEnergyBased")
        .def(py::init<>())
        .def(py::init<const core::NumericalData<double>&, const core::NumericalData<double>&>())
        .def(py::init<const SpectrumEnergyBasedPyType&>())
        .def("estimateBackground", [](const SpectrumEnergyBasedPyType& spectrum, const std::vector<int>& iteration_list){
            return spectrum.estimateBackground(iteration_list.begin(), iteration_list.end());
        })
        .def("removeBackground", [](SpectrumEnergyBasedPyType& spectrum, const std::vector<int>& iteration_list){
            spectrum.removeBackground(iteration_list.begin(), iteration_list.end());
        });

    // IO module read/write to file, etc...
    m_io.def("from_csv", 
            [](SpectrumEnergyBasedPyType& hist, const std::string& filename) {
                std::ifstream file(filename);
                //file >> m;
                io::Deserialize<double, double, ','>(file, hist);
                file.close();
            });
}