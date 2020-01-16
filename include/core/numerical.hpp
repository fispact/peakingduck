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

#include <Eigen/Core>
#include <Eigen/Dense>

#include "common.hpp"
#include "crtp.hpp"

#include "numericalmacros.h"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    const int ArrayTypeDynamic = Eigen::Dynamic;

    template<typename Scalar, int Size=ArrayTypeDynamic>
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

        void ramp(double threshold)
        {
            std::function<double(double)> imp = [&](double x){
                return (x >= threshold) ? x : 0;
            };
            this->underlying() = this->underlying().unaryExpr(imp);
        }
    };

    /*!
       @brief Represents a 1-dimensional data structure (basically a 1D Eigen array)
        Dynamic array - most use cases will be determined at runtime (I am assuming).
        We don't want anyone to know we are using Eigen beyond this file, since (in theory)
        it should make it easier to change library if need be. We only really need the array
        datastructure from Eigen and not much else and instead of reinventing the wheel,
        we wrap Eigen array.

        We wrap this with private inheritance on the Eigen type but there
        are a lot of methods to expose, easy to add when/if we need them. 

        Eigen array is pretty good, it has things like sqrt, exp on array coefficients, but 
        we need to extend this to other functions, so we use CRTP to do this.
    */
    template<typename T=double, int Size=ArrayTypeDynamic>
    class NumericalData : private Array1D<T, Size>, 
                          public NumericalFunctions<NumericalData<T, Size>>
    {
        public:
            using BaseEigenArray = Array1D<T, Size>;

            // typedef Array1Dd Base;
            using BaseEigenArray::BaseEigenArray;

            // clang does not like this, but gcc does,
            // not sure why, but maybe not needed?
            // using BaseEigenArray::Base;

            // operations such as (x > 0).all()
            using BaseEigenArray::Base::all;
            using BaseEigenArray::Base::any;
            using BaseEigenArray::Base::count;

            // essential operations on arrays
            using BaseEigenArray::operator<<;
            using BaseEigenArray::operator>;
            using BaseEigenArray::operator<;
            using BaseEigenArray::operator=;
            using BaseEigenArray::operator==;
            using BaseEigenArray::operator*;
            using BaseEigenArray::operator*=;
            using BaseEigenArray::operator+;
            using BaseEigenArray::operator+=;
            using BaseEigenArray::operator-;
            using BaseEigenArray::operator-=;
            using BaseEigenArray::operator/;
            using BaseEigenArray::operator/=;

            PEAKINGDUCK_NUMERICAL_OPERATOR_IMP_MACRO(NumericalData,BaseEigenArray,+)
            PEAKINGDUCK_NUMERICAL_OPERATOR_IMP_MACRO(NumericalData,BaseEigenArray,-)
            PEAKINGDUCK_NUMERICAL_OPERATOR_IMP_MACRO(NumericalData,BaseEigenArray,*)
            PEAKINGDUCK_NUMERICAL_OPERATOR_IMP_MACRO(NumericalData,BaseEigenArray,/)

            // entry access operations
            using BaseEigenArray::operator[];
            using BaseEigenArray::operator();
            using BaseEigenArray::data;

            // some useful predefined methods 
            // map
            using BaseEigenArray::sqrt;
            using BaseEigenArray::square;
            using BaseEigenArray::pow;
            using BaseEigenArray::reverse;
            using BaseEigenArray::reverseInPlace;

            // reduce
            using BaseEigenArray::mean;
            using BaseEigenArray::sum;
            using BaseEigenArray::maxCoeff;
            using BaseEigenArray::minCoeff;

            // custom unary operations
            using BaseEigenArray::unaryExpr;
    };

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_NUMERICAL_HPP