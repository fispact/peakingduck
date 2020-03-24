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

        ToDo: write

        A list:
            - Entry 1
            - Entry 2

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
	       R"pbdoc(
                Given a list of values take nouter points either side of 
                the index given and ignore ninner points.

                Examples:

                    ::
    
                        >>> get_window([8, 2, 5, 2, 6, 6, 9, 23, 12], 4, 3, 0, True)
                        [2, 5, 2, 6, 6, 9, 23]
                        >>> get_window([8, 2, 5, 2, 6, 6, 9, 23, 12], 4, 3, 0, False)
                        [2, 5, 2, 6, 9, 23]
                        >>> get_window([8, 2, 5, 2, 6, 6, 9, 23, 12], 4, 3, 1, True)
                        [2, 5, 6, 9, 23]
                        >>> get_window([8, 2, 5, 2, 6, 6, 9, 23, 12], 4, 3, 1, False)
                        [2, 5, 9, 23]

                Therefore:
                    - ninner >= 0
                    - ninner <= nouter
                    - index >= nouter
                    - index < values.size()
        
                It will clip at (0, len(values)))pbdoc",
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
    py::class_<NumericalDataPyType>(m_core, "NumericalData",
	       R"pbdoc(
                 Represents a 1-dimensional data structure of floats 
                 (basically a 1D Eigen array)

                 Dynamic array - most use cases will be determined at
                 runtime (I am assuming).  We don't want anyone to
                 know we are using Eigen beyond this file, since (in
                 theory) it should make it easier to change library if
                 need be. We only really need the array datastructure
                 from Eigen and not much else and instead of
                 reinventing the wheel, we wrap Eigen array.

                 We wrap this with private inheritance on the Eigen
                 type but there are a lot of methods to expose, easy
                 to add when/if we need them.
        
                 Eigen array is pretty good, it has things like sqrt,
                 exp on array coefficients, but we need to extend this
                 to other functions, so we use CRTP to do this.

                 For all of this, you may ask why not just use Eigen
                 and use an alias?  Well for one, we don't need all of
                 Eigen just the array, and not all of the array type
                 (we require a simpler interface). Additionally, at
                 some point we may wish to use another data structure
                 as std::array for example.  In this case we just
                 change the NumericalData class to wrap that instead.
                 If we change the alias this could break existing
                 interfaces and APIs, causing big changes later
                 on. Since this datastructure is fundamental to
                 everything we need to make sure that we have this
                 sorted properly first!)pbdoc")
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
        .def("maxCoeff", [](const NumericalDataPyType& data){
            return data.maxCoeff();
        })
        .def("minCoeff", [](const NumericalDataPyType& data){
            return data.minCoeff();
        })
        .def("mean", [](const NumericalDataPyType& data){
            return data.mean();
        })
        .def("stddev", [](const NumericalDataPyType& data, int ddof){
            return data.stddev(ddof);
        }, py::arg("ddof") = 0)
        .def("sum", [](const NumericalDataPyType& data){
            return data.sum();
        })
        .def("exp", [](const NumericalDataPyType& data){
            return data.exp();
        })
        .def("log", [](const NumericalDataPyType& data){
            return data.log();
        })
        .def("sqrt", [](const NumericalDataPyType& data){
            return data.sqrt();
        })
        .def("square", [](const NumericalDataPyType& data){
            return data.square();
        })
        .def("pow", [](const NumericalDataPyType& data, NumericalDataCoreType exponent){
            return data.pow(exponent);
        })
        .def("reverse", [](const NumericalDataPyType& data){
            return data.reverse();
        })
        .def("reverseInPlace", [](NumericalDataPyType& data){
            data.reverseInPlace();
        })
        .def("from_list", &NumericalDataPyType::from_vector)
        .def("to_list", &NumericalDataPyType::to_vector)
        .def("slice", &NumericalDataPyType::slice)
        .def("LLS", &NumericalDataPyType::LLS,
	     R"pbdoc(
              log(log(sqrt(value + 1) + 1) + 1)

              Returns:
                  A new array.)pbdoc")
        .def("LLSInPlace", &NumericalDataPyType::LLSInPlace,
	     R"pbdoc(
              log(log(sqrt(value + 1) + 1) + 1)

              Changes the underlying array.)pbdoc")
        .def("inverseLLS", &NumericalDataPyType::inverseLLS,
	     R"pbdoc(
               exp(exp(sqrt(value + 1) + 1) + 1)

               Returns:
                    A new array.)pbdoc")
        .def("inverseLLSInPlace", &NumericalDataPyType::inverseLLSInPlace,
	     R"pbdoc(
               exp(exp(sqrt(value + 1) + 1) + 1)

               Changes the underlying array.)pbdoc")
        .def("midpoint", &NumericalDataPyType::midpoint,
	     R"pbdoc(
              For each element calculate the midpoint value from the
              adjacent elements at a given order.

              Take the i-order point and the i+order point and
              determine the average = (array[i-j]+array[i+j])/2.0.
              End points are not counted (stay as original) - max(0,
              i-j) and min(i+j, len(array))

              Examples:
                  Given an array: [1,   4,   6, 2,   4,   2, 5]
                    - we have the midpoints for order 0:   [1,   4,   6, 2,   4,   2, 5]
                    - we have the midpoints for order 1:   [1, 3.5,   3, 5,   2, 4.5, 5]
                    - we have the midpoints for order 2:   [1,   4, 2.5, 3, 5.5,   2, 5]
                    - we have the midpoints for order 3:   [1,   4,   6, 3,   4,   2, 5]
                    - we have the midpoints for order 4+:  [1,   4,   6, 2,   4,   2, 5]

              Returns:
                  A new array.)pbdoc")
        .def("midpointInPlace", &NumericalDataPyType::midpointInPlace,
	     R"pbdoc(
              For each element calculate the midpoint value from the
              adjacent elements at a given order.

              Mutates underlying array.

              See Also:
                  :func:`peakingduck.core.NumericalData.midpoint`)pbdoc")
        .def("snip", [](const NumericalDataPyType& data, const std::vector<int>& iteration_list){
            return data.snip(iteration_list.begin(), iteration_list.end());
        }, R"pbdoc(
              Sensitive Nonlinear Iterative Peak (SNIP) algorithm for
              estimating backgrounds

              ref needed here:

              Allows any form of iterations given an iterator

              Returns:
                  A new array.)pbdoc")
        .def("snip", (NumericalDataPyType (NumericalDataPyType::*)(int) const)&NumericalDataPyType::snip,
	     R"pbdoc(
              Sensitive Nonlinear Iterative Peak (SNIP) algorithm for
              estimating backgrounds ref needed here.

              Does via increasing window only (ToDo: need to allow decreasing window)

              Deprecate this!

              Returns:
                  A new array.)pbdoc",
            py::arg("niterations") = 20)

        /* pybind cannot deduce types so we need to use old style function pointers */
        // .def("snip", py::overload_cast<std::vector<int>::iterator,std::vector<int>::iterator>(&NumericalDataPyType::snip))
        // .def("snip", py::overload_cast<int>(&NumericalDataPyType::snip),
        //     py::arg("niterations") = 20)

        .def("ramp", &NumericalDataPyType::ramp, R"pbdoc(
              A simple function for filtering values above a certain
              threshold (>=). This is useful to remove entries that
              are negative for example.

              Returns:
                  A new array.)pbdoc")
        .def("rampInPlace", &NumericalDataPyType::rampInPlace, R"pbdoc(
              A simple function for filtering values above a certain
              threshold (>=). This is useful to remove entries that
              are negative for example.

              Mutates underlyindg data.)pbdoc");

    m_core.def("combine", &core::combine<NumericalDataCoreType,core::ArrayTypeDynamic>);
    m_core.def("window", &core::window<NumericalDataCoreType,core::ArrayTypeDynamic>,
            py::arg("values"),
            py::arg("centerindex"),
            py::arg("nouter") = 5,
            py::arg("ninner") = 0,
            py::arg("includeindex") = true);

    // integral type
    using IntegerDataCoreType = int;
    using IntegerDataPyType = core::NumericalData<IntegerDataCoreType,core::ArrayTypeDynamic>;
    py::class_<IntegerDataPyType>(m_core, "IntegerData",
		 R"pbdoc(
                 Represents a 1-dimensional data structure of ints
                 (basically a 1D Eigen array)

                 Dynamic array - most use cases will be determined at
                 runtime (I am assuming).  We don't want anyone to
                 know we are using Eigen beyond this file, since (in
                 theory) it should make it easier to change library if
                 need be. We only really need the array datastructure
                 from Eigen and not much else and instead of
                 reinventing the wheel, we wrap Eigen array.

                 We wrap this with private inheritance on the Eigen
                 type but there are a lot of methods to expose, easy
                 to add when/if we need them.
        
                 Eigen array is pretty good, it has things like sqrt,
                 exp on array coefficients, but we need to extend this
                 to other functions, so we use CRTP to do this.

                 For all of this, you may ask why not just use Eigen
                 and use an alias?  Well for one, we don't need all of
                 Eigen just the array, and not all of the array type
                 (we require a simpler interface). Additionally, at
                 some point we may wish to use another data structure
                 as std::array for example.  In this case we just
                 change the NumericalData class to wrap that instead.
                 If we change the alias this could break existing
                 interfaces and APIs, causing big changes later
                 on. Since this datastructure is fundamental to
                 everything we need to make sure that we have this
                 sorted properly first!)pbdoc")
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
        .def("maxCoeff", [](const IntegerDataPyType& data){
            return data.maxCoeff();
        })
        .def("minCoeff", [](const IntegerDataPyType& data){
            return data.minCoeff();
        })
        .def("sum", [](const IntegerDataPyType& data){
            return data.sum();
        })
        .def("reverse", [](const IntegerDataPyType& data){
            return data.reverse();
        })
        .def("reverseInPlace", [](IntegerDataPyType& data){
            data.reverseInPlace();
        })
        .def("slice", &IntegerDataPyType::slice)
        .def("from_list", &IntegerDataPyType::from_vector)
        .def("to_list", &IntegerDataPyType::to_vector)
      .def("ramp", &IntegerDataPyType::ramp, R"pbdoc(
              A simple function for filtering values above a certain
              threshold (>=). This is useful to remove entries that
              are negative for example.

              Returns:
                  A new array.)pbdoc")
      .def("rampInPlace", &IntegerDataPyType::rampInPlace, R"pbdoc(
              A simple function for filtering values above a certain
              threshold (>=). This is useful to remove entries that
              are negative for example.

              Mutates underlyindg data.)pbdoc");

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

    py::class_<IProcessPyType, PyProcess, std::shared_ptr<IProcessPyType>>(m_core, "IProcess",
		 R"pbdoc(
                 Interface for all process algorithms

                 Operates on numerical data.
                 Never mutates the input (always const process).

                 Returns:
                     A new numerical array.)pbdoc")
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

    py::class_<IProcessManagerPyType, PyProcessManager, std::shared_ptr<IProcessManagerPyType>>(m_core, "IProcessManager", "A general process manager interface")
        .def(py::init_alias<>())
        .def("append", &IProcessManagerPyType::append)
        .def("run", &IProcessManagerPyType::run)
        .def("__len__", &IProcessManagerPyType::size)
        .def("reset", &IProcessManagerPyType::reset);

    // simple process manager
    using SimpleProcessManagerPyType = core::SimpleProcessManager<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<SimpleProcessManagerPyType, IProcessManagerPyType, std::shared_ptr<SimpleProcessManagerPyType>>(m_core, "SimpleProcessManager", "A simple process manager")
        .def(py::init<>())
        .def("append", &SimpleProcessManagerPyType::append)
        .def("run", &SimpleProcessManagerPyType::run)
        .def("__len__", &SimpleProcessManagerPyType::size)
        .def("reset", &SimpleProcessManagerPyType::reset);

    // smoothing objects
    using MovingAverageSmootherPyType = core::MovingAverageSmoother<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<MovingAverageSmootherPyType, IProcessPyType, std::shared_ptr<MovingAverageSmootherPyType>>(m_core, "MovingAverageSmoother",
		 R"pbdoc(
                  Simple moving average smoother

                  Can we support windowsize given at compile time too?
                  It would certainly help with unit tests.)pbdoc")
        .def(py::init<int>());

    using WeightedMovingAverageSmootherPyType = core::WeightedMovingAverageSmoother<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<WeightedMovingAverageSmootherPyType, IProcessPyType, std::shared_ptr<WeightedMovingAverageSmootherPyType>>(m_core, "WeightedMovingAverageSmoother",
		 R"pbdoc(
                  Simple moving average smoother

                 Uses weights determined, with windowsize = N
                     - if N=1, weights=[1] -> [1/N]
                     - if N=2, weights=[1,1] -> [1/2, 1/2]
                     - if N=3, weights=[1,2,1] -> [1/4, 2/4, 1/4]
                     - if N=4, weights=[1,2,2,1] -> [1/6, 2/6, 2/6, 1/6]
                     - if N=5, weights=[1,2,3,2,1] -> [1/9, 2/9, 3/9, 2/9, 1/9]
                     - ...)pbdoc")
        .def(py::init<int>());

    // peak filter objects
    using GlobalThresholdPeakFilterPyType = core::GlobalThresholdPeakFilter<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<GlobalThresholdPeakFilterPyType, IProcessPyType, std::shared_ptr<GlobalThresholdPeakFilterPyType>>(m_core, "GlobalThresholdPeakFilter", "Simple threshold global peak filter")
        .def(py::init<NumericalDataCoreType>());

    using ChunkedThresholdPeakFilterPyType = core::ChunkedThresholdPeakFilter<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<ChunkedThresholdPeakFilterPyType, IProcessPyType, std::shared_ptr<ChunkedThresholdPeakFilterPyType>>(m_core, "ChunkedThresholdPeakFilter", "Simple threshold local/chunked peak filter")
        .def(py::init<NumericalDataCoreType, size_t>(), 
            py::arg("percentThreshold"), 
            py::arg("chunkSize") = 10);

    using MovingAveragePeakFilterPyType = core::MovingAveragePeakFilter<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<MovingAveragePeakFilterPyType, IProcessPyType, std::shared_ptr<MovingAveragePeakFilterPyType>>(m_core, "MovingAveragePeakFilter", "Simple moving average peak filter")
        .def(py::init<int>());

    // peak info struct
    using PeakInfoPyType = core::PeakInfo<NumericalDataCoreType>;
    py::class_<PeakInfoPyType>(m_core, "PeakInfo" , R"pbdoc(
                 Simple struct for holding peak info.
                 
                 Attributes:
                     value: value, i.e. count or flux.
                     index: the corresponding index/channel of the data.)pbdoc")
        .def(py::init<size_t, NumericalDataCoreType>(),
            py::arg("index") = 0,
            py::arg("value") = 0.0)
        .def_property_readonly("index", [](const PeakInfoPyType& peak){
            return peak.index;
        })
        .def_property_readonly("value", [](const PeakInfoPyType& peak){
            return peak.value;
        });

    // core peak finding interface
    using IPeakFinderPyType = core::IPeakFinder<NumericalDataCoreType,core::ArrayTypeDynamic>;

    class PyPeakFinder : public IPeakFinderPyType {
        public:
            /* Inherit the constructors */
            using IPeakFinderPyType::IPeakFinder;

            /* Trampoline (need one for each virtual function) */
            core::PeakList<NumericalDataCoreType>
            find(const NumericalDataPyType& data) const override {
                PYBIND11_OVERLOAD_PURE(
                    core::PeakList<NumericalDataCoreType>,      /* Return type */
                    IPeakFinderPyType,                          /* Parent class */
                    find,                                       /* Name of function in C++ (must match Python name) */
                    data                                        /* Argument(s) */
                );
            }
    };

    py::class_<IPeakFinderPyType, PyPeakFinder, std::shared_ptr<IPeakFinderPyType>>(m_core, "IPeakFinder",
                R"pbdoc(
                 Interface for peak finding algorithms

                 Operates on numerical data (filtered or
                 unfiltered). Never mutates the input (always const
                 process).

                 Returns:
                     PeakList:  A list of peaks)pbdoc")
        .def(py::init_alias<>())
    .def("find", &IPeakFinderPyType::find,
	 "Identifies potential peaks in the data");

    // peak finder objects
    using SimplePeakFinderPyType = core::SimplePeakFinder<NumericalDataCoreType,core::ArrayTypeDynamic>;
    py::class_<SimplePeakFinderPyType, IPeakFinderPyType, std::shared_ptr<SimplePeakFinderPyType>>(m_core, "SimplePeakFinder")
        .def(py::init<NumericalDataCoreType>(), 
            py::arg("threshold") = 0)
        .def("find", &SimplePeakFinderPyType::find);

    // histogram objects
    using HistPyType = core::Histogram<double,double>;
    using HistChannelPyType = core::Histogram<int,double>;

    py::class_<HistPyType>(m_core, "Histogram", R"pbdoc(
                 Represents a basic 1D histogram
                 
                 Energies vs values.)pbdoc")
        .def(py::init<>())
        .def(py::init<const core::NumericalData<double>&, const core::NumericalData<double>&>())
        .def(py::init<const HistPyType&>())
        .def_property_readonly("X", &HistPyType::X)
        .def_property_readonly("Y", &HistPyType::Y);

    py::class_<HistChannelPyType>(m_core, "HistogramChannelBased",  R"pbdoc(
                 Represents a basic 1D histogram
                 
                 Channels vs values.)pbdoc")
        .def(py::init<>())
        .def(py::init<const core::NumericalData<int>&, const core::NumericalData<double>&>())
        .def(py::init<const HistChannelPyType&>())
        .def_property_readonly("X", &HistChannelPyType::X)
        .def_property_readonly("Y", &HistChannelPyType::Y);

    // spectrum objects
    using SpectrumChannelBasedPyType = core::Spectrum<int,double>;
    py::class_<SpectrumChannelBasedPyType, HistChannelPyType>(m_core, "SpectrumChannelBased",
                R"pbdoc(
                 Represents a basic 1D histogram
                 
                 Channels vs values.)pbdoc")
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
    py::class_<SpectrumEnergyBasedPyType, HistPyType>(m_core, "SpectrumEnergyBased",
                R"pbdoc(
                 Represents a basic 1D histogram
                 
                 Energies vs values.)pbdoc")
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
            }, R"pbdoc(
                 Deserialization method for histogram
                 
                 Assumes delimited text data in column form of:
                 
                 ::
                 
                     channel, lowerenergy, upperenergy, count
                 )pbdoc");
}
