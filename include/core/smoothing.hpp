////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines smoothing functions to be applied to numerical array

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef CORE_SMOOTHING_HPP
#define CORE_SMOOTHING_HPP

#include "common.hpp"
#include "numerical.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Interface for smoothing algorithms
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct ISmoother
    {
        virtual NumericalData<T, Size> 
        operator()(const NumericalData<T, Size>& data) const = 0;
    };    

    /*!
       @brief Simple moving average smoother
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct MovingAverageSmoother : public ISmoother<T, Size>
    {

        explicit MovingAverageSmoother(int windowsize) : _windowsize(windowsize)
        {}

        NumericalData<T, Size> 
        operator()(const NumericalData<T, Size>& data) const override final
        {
            NumericalData<T, Size> smoothed = data;

            // loop over array and get the mean from nearby points
            for(int i=_windowsize; i<data.size()-_windowsize; ++i){
                smoothed[i] = data(i-_windowsize, i+_windowsize+1).mean();
            }

            return smoothed;
        };

      private:
        const int _windowsize;
    };  

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_SMOOTHING_HPP