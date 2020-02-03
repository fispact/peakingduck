////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines peak finding functions to be applied to numerical array

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef CORE_PEAKING_HPP
#define CORE_PEAKING_HPP

#include <cmath>
#include <memory>

#include "common.hpp"
#include "core/numerical.hpp"
#include "core/process.hpp"
#include "core/smoothing.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Simple moving average peak finding 
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct MovingAveragePeakFinder : public IProcess<T, Size>
    {

        explicit MovingAveragePeakFinder(int windowsize)
        {
            _movingAverageSmoother = std::make_shared<MovingAverageSmoother<T,Size>>(windowsize);
        }

        NumericalData<T, Size> 
        go(const NumericalData<T, Size>& data) const override final
        {
            NumericalData<T, Size> smoothed = data - _movingAverageSmoother->go(data);
            for(int i=0;i<data.size();++i)
                smoothed[i] = smoothed[i] > 0 ? smoothed[i] : 0.0;
            return smoothed;
        };

      private:
        std::shared_ptr<IProcess<T,Size>> _movingAverageSmoother;
    };  


PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_PEAKING_HPP