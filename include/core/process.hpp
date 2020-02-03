////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines a general processor and manager to handle a sequence of
    operations on numerical data

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef CORE_PROCESS_HPP
#define CORE_PROCESS_HPP

#include <memory>
#include <vector>

#include "common.hpp"
#include "core/numerical.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Interface for all process algorithms

       Operates on numerical data
       Never mutates the input (always const process)
       returns a new numerical array
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct IProcess
    {
        virtual ~IProcess(){};
        
        virtual NumericalData<T, Size> 
        go(const NumericalData<T, Size>& data) const = 0;
    };    

    /*!
       @brief A general process manager interface
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct IProcessManager{

        virtual ~IProcessManager(){};

        // return reference to allow chaining
        virtual IProcessManager& append(const std::shared_ptr<IProcess<T, Size>>& process) = 0;

        virtual NumericalData<T, Size> 
        run(const NumericalData<T, Size>& data) const = 0;

        virtual size_t size() const = 0;

        virtual void reset() = 0;
    };

    /*!
       @brief A simple process manager
    */
    template<typename T=DefaultType, int Size=ArrayTypeDynamic>
    struct SimpleProcessManager : public IProcessManager<T,Size>{

        virtual ~SimpleProcessManager(){};

        // return reference to allow chaining
        IProcessManager<T,Size>& append(const std::shared_ptr<IProcess<T, Size>>& process) override{
            _processes.push_back(process);
            return *this;
        }

        NumericalData<T, Size> 
        run(const NumericalData<T, Size>& data) const override{
            // can we avoid a copy here?
            NumericalData<T, Size> processedData = data;
            for(auto& process: _processes){
                processedData = process->go(processedData);
            }
            return processedData;
        }

        inline size_t size() const override{
            return _processes.size();
        }

        void reset() override{
            _processes.resize(0);
        }

      private:
        std::vector<std::shared_ptr<IProcess<T, Size>>> _processes;
    };

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_PROCESS_HPP