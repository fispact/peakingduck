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
       @brief Simple threshold global peak finder
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct GlobalThresholdPeakFinder : public IProcess<T, Size>
    {

        explicit GlobalThresholdPeakFinder(T percentThreshold) : _percentThreshold(percentThreshold)
        {
        }

        NumericalData<T, Size> 
        go(const NumericalData<T, Size>& data) const override final
        {
            const T absThreshold = data.maxCoeff()*_percentThreshold;
            NumericalData<T, Size> processed = data.ramp(absThreshold);
            // ToDo: check clusters and take the max value within each cluster
            return processed;
        };

      private:
        const T _percentThreshold;
    };  

    /*!
       @brief Simple threshold local/chunked peak finder
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct ChunkedThresholdPeakFinder : public IProcess<T, Size>
    {

        explicit ChunkedThresholdPeakFinder(T percentThreshold, size_t chunkSize=10) 
        : _percentThreshold(percentThreshold), _chunkSize(chunkSize)
        {
        }

        NumericalData<T, Size> 
        go(const NumericalData<T, Size>& data) const override final
        {
            // copy input data for output processing
            NumericalData<T, Size> processed = NumericalData<T, Size>::Zero(data.size());

            // get indicies for chunking
            auto chunkindices = util::rrange<size_t>(0, data.size(), _chunkSize);

            // use the global threshold peak finder on each chunk
            const GlobalThresholdPeakFinder<T> gpf(_percentThreshold);

            // iterate over chunks and apply peak finding
            // what if list is empty?            
            for(auto it=chunkindices.begin(); it!=std::prev(chunkindices.end()); ++it){
                // slice it and apply the threshold
                auto newdata = gpf.go(data(*it, std::min(*std::next(it, _chunkSize), *chunkindices.end())));
                // copy over data
                for(int i=0; i<newdata.size();++i){
                    processed[*it+i] = newdata[i];
                }
            }
            return processed;
        };

      private:
        const T _percentThreshold;
        const size_t _chunkSize;
    };  

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