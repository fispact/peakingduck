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
#include <vector>

#include "common.hpp"
#include "core/numerical.hpp"
#include "core/process.hpp"
#include "core/smoothing.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Simple threshold global peak filter
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct GlobalThresholdPeakFilter : public IProcess<T, Size>
    {

        explicit GlobalThresholdPeakFilter(T percentThreshold) : _percentThreshold(percentThreshold)
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
       @brief Simple threshold local/chunked peak filter
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct ChunkedThresholdPeakFilter : public IProcess<T, Size>
    {

        explicit ChunkedThresholdPeakFilter(T percentThreshold, size_t chunkSize=10) 
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

            // use the global threshold peak filter on each chunk
            const GlobalThresholdPeakFilter<T> gpf(_percentThreshold);

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
       @brief Simple moving average peak filter 
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct MovingAveragePeakFilter : public IProcess<T, Size>
    {

        explicit MovingAveragePeakFilter(int windowsize)
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

    /*!
       @brief Simple struct for holding peak info

       Stores the:
        value    = value i.e. count, flux
        index    = the corresponding index/channel in the data

    */
    template<typename ValueType=DefaultType>
    struct PeakInfo
    {
        const size_t index;
        const ValueType value;

        PeakInfo(size_t pindex, ValueType pvalue) : index(pindex), value(pvalue) {}
    };

    /*!
       @brief Struct extends basic PeakInfo with a property value

       Stores the:
        property = property i.e. energy, time
        value    = value i.e. count, flux
        index    = the corresponding index/channel in the data

    */
    template<typename ValueType=DefaultType, typename PropType=DefaultType>
    struct PeakInfoWithProp : public PeakInfo<ValueType>
    {
        const PropType property;

        PeakInfoWithProp(size_t pindex, ValueType pvalue, PropType pprop) : PeakInfo<ValueType>(pindex, pvalue), property(pprop) {}
    };

    // it is not immediatly clear to me how to make this container
    // for PeakInfo polymorphic, i.e. how can we keep the container the same
    // for different types?
    // Since everything must be runtime polymorphic (python bindings), it leads
    // us to use interfaces and vtables but what about cases where
    // we don't want or even know the property? Cannot force property in the interface.
    // for now we just use value and index and properties can be introduced later 
    // if you have the index
    template<typename ValueType=DefaultType>
    using PeakList = std::vector<PeakInfo<ValueType>>;

    /*!
       @brief Interface for peak finding algorithms

       Operates on numerical data (filtered or unfiltered)
       Never mutates the input (always const process)
       returns a list of peaks - PeakList
    */
    template<typename ValueType=DefaultType, 
             int Size=ArrayTypeDynamic>
    struct IPeakFinder
    {
        virtual ~IPeakFinder(){};
        
        /*!
           @brief Identifies potential peaks in the data
        */
        virtual PeakList<ValueType>
        find(const NumericalData<ValueType, Size>& data) const = 0;

        // What else should this do?
        // If find is a slow process should we allow interface to provide
        // get last values? 
        // nrofpeaks? getstored?
    };    

    /*!
       @brief Interface for peak finding algorithms

       Operates on numerical data (filtered or unfiltered)
       Never mutates the input (always const process)
       returns a list of peaks - PeakList
    */
    template<typename ValueType=DefaultType, 
             int Size=ArrayTypeDynamic>
    struct SimplePeakFinder : public IPeakFinder<ValueType, Size>
    {
        SimplePeakFinder(ValueType percentThreshold) :
            _percentThreshold(percentThreshold)
        {
        };

        virtual ~SimplePeakFinder()
        {
        };
        
        /*!
           @brief Identifies potential peaks in the data based on a simple
           global threshold of max coefficient
        */
        virtual PeakList<ValueType>
        find(const NumericalData<ValueType, Size>& data) const override{

            const ValueType relativeThreshold = data.maxCoeff()*_percentThreshold;
            
            // find consecutive bins for a peak group (indices)
            using PeakGroup = std::vector<size_t>;

            // identify values greater than the threshold
            // and store their consecutive indices
            PeakGroup local_peak;
            std::vector<PeakGroup> peak_groups;

            // can we use the STL instead - maybe std::partition?
            int last_index = -1;
            for(auto i=0; i<data.size(); ++i){
                if(data[i] > relativeThreshold){
                    if((last_index + 1 < i) && (last_index >=0)){
                        peak_groups.emplace_back(local_peak);
                        local_peak.resize(0);
                    }
                    local_peak.emplace_back(i);
                    last_index = i;
                }
            }

            if(local_peak.size() > 0){
                peak_groups.emplace_back(local_peak);
            }

            PeakList<ValueType> peaks;

            // find maximum in each group
            size_t max_index = -1;
            ValueType max_value = -1;
            for(auto& group: peak_groups){
                max_value = -1.0;
                max_index = -1;
                for(auto i: group){
                    if(data[i] > max_value){
                        max_value = data[i];
                        max_index = i;
                    }
                }

                peaks.emplace_back(PeakInfo<ValueType>(max_index, data[max_index]));
            }

            return peaks;
        }
        
      private:
        const ValueType _percentThreshold;
    };    

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_PEAKING_HPP