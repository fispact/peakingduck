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

#include <cmath>

#include "common.hpp"
#include "core/numerical.hpp"
#include "core/process.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Simple moving average smoother

        Can we support windowsize given at compile time too?
        It would certainly help with unit tests.
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct MovingAverageSmoother : public IProcess<T, Size>
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

    /*!
       @brief Weighted moving average smoother

       Uses weights determined, with windowsize = N
            if N=1, weights=[1] -> [1/N]
            if N=2, weights=[1,1] -> [1/2, 1/2]
            if N=3, weights=[1,2,1] -> [1/4, 2/4, 1/4]
            if N=4, weights=[1,2,2,1] -> [1/6, 2/6, 2/6, 1/6]
            if N=5, weights=[1,2,3,2,1] -> [1/9, 2/9, 3/9, 2/9, 1/9]
            ....

    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct WeightedMovingAverageSmoother : public IProcess<T, Size>
    {
        explicit WeightedMovingAverageSmoother(int windowsize) : _windowsize(windowsize)
        {
            // weights should be of size windowsize*2 + 1
            // because we take windowsize either side and then including 
            // the current point
            _weights = NumericalData<T>((_windowsize*2)+1);

            // determine weights at construction
            for(int i=0;i<ceil(_windowsize/2.0);++i){
                _weights[i] = static_cast<double>(i+1);
            }
            for(int i=floor(_windowsize/2.0);i>0;--i){
                _weights[_windowsize-i] = static_cast<double>(i);                
            }

            // scale by the sum
            _weights *= 1.0/_weights.sum();
        }

        NumericalData<T, Size> 
        operator()(const NumericalData<T, Size>& data) const override final
        {
            NumericalData<T, Size> smoothed = data;

            // loop over array and get the mean from nearby points
            for(int i=_windowsize; i<data.size()-_windowsize; ++i){
                smoothed[i] = (data(i-_windowsize, i+_windowsize+1)*_weights).mean();
            }

            return smoothed;
        };

      private:
        const int _windowsize;
        NumericalData<T> _weights;
    };  

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_SMOOTHING_HPP