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
#ifndef CORE_NUMERICAL_FUNCTIONS_HPP
#define CORE_NUMERICAL_FUNCTIONS_HPP

#include "common.hpp"
#include "crtp.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief To extend the NumericalData type with certain numerical 
       abilities, we add it to this using CRTP to keep it in the interface 
       but it doesn't require us to change the underlying data structure.
    */
    template <class Derived>
    struct NumericalFunctions : crtp<Derived, NumericalFunctions>
    {

        /*!
            @brief log(log(sqrt(value + 1) + 1) + 1)
            Returns a new array
        */
        Derived LLS() const
        {
            return (((this->underlying() + 1.0).sqrt() + 1.0).log() + 1.0).log();
        }

        /*!
            @brief log(log(sqrt(value + 1) + 1) + 1)
            Changes the underlying array
        */
        Derived& LLSInPlace()
        {
            this->underlying() = this->underlying().LLS();
            return this->underlying();
        }

        /*!
            @brief exp(exp(sqrt(value + 1) + 1) + 1)
            Returns a new array
        */
        Derived inverseLLS() const
        {
            return ((((this->underlying().exp() - 1.0).exp()) - 1.0).square()) - 1.0;
        }

        /*!
            @brief exp(exp(sqrt(value + 1) + 1) + 1)
            Changes the underlying array
        */
        Derived& inverseLLSInPlace()
        {
            this->underlying() = this->underlying().inverseLLS();
            return this->underlying();
        }
        
        /*!
            @brief For each element calculate a new value from the symmetric neigbour values
            at a given order. 
            Take the i-order point and the i+order point and apply a function to that value
            End points are not counted (stay as original) - max(0, i-j) and min(i+j, len(array))

            Returns a new array
        */
        Derived symmetricNeighbourOp(const std::function<void(int, int, const Derived&, Derived&)>& operation, int order=1) const
        {            
            // perform algorithm between these two indices
            const int istart = order;
            const int iend = this->underlying().size() - order;

            // can we do this without copying twice, or even once?
            // since we cannot change in place for each entry in loop as this
            // changes results for other entries later.
            Derived newvalues = this->underlying();
            for(int i=istart; i<iend; ++i){
                operation(i, order, this->underlying(), newvalues);
            }            
            return newvalues;
        }

        /*!
            @brief For each element calculate the midpoint value from the adjacent elements at a given 
            order. 
            Take the i-order point and the i+order point and determine the average = (array[i-j]+array[i+j])/2.0.
            End points are not counted (stay as original) - max(0, i-j) and min(i+j, len(array))

            For example, given an array:         [1,   4,   6, 2,   4,   2, 5]
            we have the midpoints for order 0:   [1,   4,   6, 2,   4,   2, 5]
            we have the midpoints for order 1:   [1, 3.5,   3, 5,   2, 4.5, 5]
            we have the midpoints for order 2:   [1,   4, 2.5, 3, 5.5,   2, 5]
            we have the midpoints for order 3:   [1,   4,   6, 3,   4,   2, 5]
            we have the midpoints for order 4+:  [1,   4,   6, 2,   4,   2, 5]

            Returns a new array
        */
        Derived midpoint(int order=1) const
        {            
            auto midpointOp = [](int i, int order, const Derived& values, Derived& newValues){
                newValues[i] = (values[i-order] + values[i+order])/2.0;
            };
            return this->underlying().symmetricNeighbourOp(midpointOp, order);
        }        

        /*!
            @brief For each element calculate the midpoint value from the adjacent elements at a given 
            order. 
            Take the i-order point and the i+order point and determine the average = (array[i-j]+array[i+j])/2.0.
            End points are not counted (stay as original) - max(0, i-j) and min(i+j, len(array))

            For example, given an array:         [1,   4,   6, 2,   4,   2, 5]
            we have the midpoints for order 0:   [1,   4,   6, 2,   4,   2, 5]
            we have the midpoints for order 1:   [1, 3.5,   3, 5,   2, 4.5, 5]
            we have the midpoints for order 2:   [1,   4, 2.5, 3, 5.5,   2, 5]
            we have the midpoints for order 3:   [1,   4,   6, 3,   4,   2, 5]
            we have the midpoints for order 4+:  [1,   4,   6, 2,   4,   2, 5]

            Mutates underlying array
        */
        Derived& midpointInPlace(int order=1)
        {           
            this->underlying() = this->underlying().midpoint(order);
            return this->underlying();
        }      

        /*!
            @brief Sensitive Nonlinear Iterative Peak (SNIP) algorithm for removing backgrounds
            ref needed here:

            does via increasing window only (ToDo: need to allow decreasing window)

            Returns a new array
        */
        Derived snip(int niterations) const
        { 
            auto midpointMinOp = [](int i, int order, const Derived& values, Derived& newValues){
                newValues[i] = (values[i-order] + values[i+order])/2.0;
                newValues[i] = std::min(newValues[i], values[i]);
            };

            Derived snipped = this->underlying();

            // first scale by LLS
            snipped.LLSInPlace();

            // iterate over iterations from 1 to niterations
            for(int i=0;i<niterations;++i){
                snipped = snipped.symmetricNeighbourOp(midpointMinOp, i+1);
            }

            // lastly scale it back LLS
            snipped.inverseLLSInPlace();

            return snipped;
        }

        /*!
            @brief A simple function for filtering values above a certain
            threshold (>=). This is useful to remove entries that are negative 
            for example.

            Returns a new array
        */
        Derived ramp(double threshold) const
        {
            std::function<double(double)> imp = [&](double x){
                return (x >= threshold) ? x : 0;
            };
            return this->underlying().unaryExpr(imp);
        }

        /*!
            @brief A simple function for filtering values above a certain
            threshold (>=). This is useful to remove entries that are negative 
            for example.

            Mutates underlying array
        */
        Derived& rampInPlace(double threshold)
        {
            this->underlying() = this->underlying().ramp(threshold);
            return this->underlying();
        }
    };

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_NUMERICAL_FUNCTIONS_HPP