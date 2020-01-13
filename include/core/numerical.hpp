////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the core numerical operations for all data manipulation within of the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef CORE_NUMERICAL_HPP
#define CORE_NUMERICAL_HPP

#include <Eigen/Dense>

#include "common.hpp"
#include "crtp.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Represents a 1-dimensional data structure (basically a 1D Eigen array)
        Dynamic array - most use cases will be determined at runtime (I am assuming)

        It would be better to wrap this (composition or private inheritance) but there
        are a lot of methods to expose. For now we simply alias the type but long term we
        need to wrap it.

        Eigen array is pretty good, it has things like sqrt, exp on array coefficients, but 
        we need to extend this to other functions, so we use CRTP.
    */
    template<typename Scalar, int Size=Eigen::Dynamic>
    using Array1D = Eigen::Array<Scalar, Size, 1>;

    using Array1Di = Array1D<int>;
    using Array1Df = Array1D<float>;
    using Array1Dd = Array1D<double>;
  
    /*!
       @brief To extend the NumericalData type with certain numerical 
       abilities, we add it to this using CRTP to keep it in the interface 
       but it doesn't require us to change the underlying data structure.
    */
    template <class Derived>
    struct NumericalFunctions : crtp<Derived, NumericalFunctions>
    {
        void scale(double multiplicator)
        {
            this->underlying() = this->underlying() * multiplicator;
        }
    };

    /*!
       @brief Represents a 1-dimensional data structure (basically a 1D Eigen array)
        Dynamic array - most use cases will be determined at runtime (I am assuming)

        We wrap this with private inheritance on the Eigen type but there
        are a lot of methods to expose, easy to add when/if we need them. 

        Eigen array is pretty good, it has things like sqrt, exp on array coefficients, but 
        we need to extend this to other functions, so we use CRTP to do this.

        ToDo: Template array type on this - how??
    */
    class NumericalData : private Array1Dd, 
                          public NumericalFunctions<NumericalData>
    {
        public:
            using Array1Dd::Array1Dd;
            using Array1Dd::operator<<;
            using Array1Dd::operator=;
            using Array1Dd::operator*;
            using Array1Dd::operator[];
            using Array1Dd::operator();

            // some useful predefined methods from 
            using Array1Dd::sqrt;
            using Array1Dd::square;
            using Array1Dd::mean;
            using Array1Dd::max;
            using Array1Dd::min;
    };

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_NUMERICAL_HPP