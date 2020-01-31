////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////


#include <memory>

#include "catch.hpp"

#include "common.hpp"

#include "peakingduck.hpp"


PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(unittests)

    template<typename T=core::DefaultType, int Size=core::ArrayTypeDynamic>
    struct AddOneProcess : public core::IProcess<T,Size>
    {
        ~AddOneProcess(){};
        
        core::NumericalData<T, Size> 
        operator()(const core::NumericalData<T, Size>& data) const{
            return data + 1;
        }
    };    

    template<typename T=core::DefaultType, int Size=core::ArrayTypeDynamic>
    struct MultiplyByTwoProcess : public core::IProcess<T,Size>
    {
        ~MultiplyByTwoProcess(){};
        
        core::NumericalData<T, Size> 
        operator()(const core::NumericalData<T, Size>& data) const{
            return data*2;
        }
    };    

    SCENARIO( "Test the process manager - int, fixed" ) {
        const int SIZE = 5;
        const core::NumericalData<int, SIZE> data(3, 5, 4, 12, 23);
        const core::NumericalData<int, SIZE> expected(9, 13, 11, 27, 49);

        auto pm = core::SimpleProcessManager<int, SIZE>();
        pm.append(std::make_shared<AddOneProcess<int, SIZE>>())
          .append(std::make_shared<MultiplyByTwoProcess<int, SIZE>>())
          .append(std::make_shared<AddOneProcess<int, SIZE>>());  

        const core::NumericalData<int, SIZE> returned = pm.run(data); 
               
        THEN( "check split" ) {
            REQUIRE( returned.size() == SIZE);
            REQUIRE( returned[0] == expected[0]);
            REQUIRE( returned[1] == expected[1]);
            REQUIRE( returned[2] == expected[2]);
            REQUIRE( returned[3] == expected[3]);
            REQUIRE( returned[4] == expected[4]);
        }        
    }

    SCENARIO( "Test the process manager v2 - int, fixed" ) {
        const int SIZE = 5;
        const core::NumericalData<int, SIZE> data(3, 5, 4, 12, 23);
        const core::NumericalData<int, SIZE> expected(10, 14, 12, 28, 50);

        auto pm = core::SimpleProcessManager<int, SIZE>();
        pm.append(std::make_shared<AddOneProcess<int, SIZE>>())
          .append(std::make_shared<AddOneProcess<int, SIZE>>())
          .append(std::make_shared<MultiplyByTwoProcess<int, SIZE>>());  

        const core::NumericalData<int, SIZE> returned = pm.run(data); 
               
        THEN( "check split" ) {
            REQUIRE( returned.size() == SIZE);
            REQUIRE( returned[0] == expected[0]);
            REQUIRE( returned[1] == expected[1]);
            REQUIRE( returned[2] == expected[2]);
            REQUIRE( returned[3] == expected[3]);
            REQUIRE( returned[4] == expected[4]);
        }        
    }

    SCENARIO( "Test the process manager - int, dynamic" ) {
        core::NumericalData<int> data(5);
        data << 3, 5, 4, 12, 23;
        core::NumericalData<int> expected(5);
        expected << 9, 13, 11, 27, 49;

        auto pm = core::SimpleProcessManager<int>();
        pm.append(std::make_shared<AddOneProcess<int>>())
          .append(std::make_shared<MultiplyByTwoProcess<int>>())
          .append(std::make_shared<AddOneProcess<int>>());  

        const core::NumericalData<int> returned = pm.run(data); 
               
        THEN( "check split" ) {
            REQUIRE( returned.size() == 5);
            REQUIRE( expected.size() == 5);
            REQUIRE( returned[0] == expected[0]);
            REQUIRE( returned[1] == expected[1]);
            REQUIRE( returned[2] == expected[2]);
            REQUIRE( returned[3] == expected[3]);
            REQUIRE( returned[4] == expected[4]);
        }        
    }

    SCENARIO( "Test the process manager - double, fixed" ) {
        const int SIZE = 5;
        const core::NumericalData<double, SIZE> data(3.5, 5, 4.1, 12.6, 23.01);
        const core::NumericalData<double, SIZE> expected(11, 14, 12.2, 29.2, 50.02);

        auto pm = core::SimpleProcessManager<double, SIZE>();
        pm.append(std::make_shared<AddOneProcess<double, SIZE>>())
          .append(std::make_shared<AddOneProcess<double, SIZE>>())
          .append(std::make_shared<MultiplyByTwoProcess<double, SIZE>>());  

        const core::NumericalData<double, SIZE> returned = pm.run(data); 
               
        THEN( "check split" ) {
            REQUIRE( pm.size() == 3);
            REQUIRE( returned.size() == SIZE);
            REQUIRE( returned[0] == expected[0]);
            REQUIRE( returned[1] == expected[1]);
            REQUIRE( returned[2] == expected[2]);
            REQUIRE( returned[3] == expected[3]);
            REQUIRE( returned[4] == expected[4]);
            THEN( "reset" ){
                // should equal original
                pm.reset();
                REQUIRE( pm.size() == 0);
                const core::NumericalData<double, SIZE> returned2 = pm.run(data); 
                REQUIRE( returned2.size() == SIZE);
                REQUIRE( returned2[0] == data[0]);
                REQUIRE( returned2[1] == data[1]);
                REQUIRE( returned2[2] == data[2]);
                REQUIRE( returned2[3] == data[3]);
                REQUIRE( returned2[4] == data[4]);
                THEN( "append new process" ){
                    pm.append(std::make_shared<AddOneProcess<double, SIZE>>());
                    REQUIRE( pm.size() == 1);
                    const core::NumericalData<double, SIZE> returned2 = pm.run(data); 
                    const core::NumericalData<double, SIZE> expected(4.5, 6, 5.1, 13.6, 24.01);
                    REQUIRE( returned2.size() == SIZE);
                    REQUIRE( returned2[0] == expected[0]);
                    REQUIRE( returned2[1] == expected[1]);
                    REQUIRE( returned2[2] == expected[2]);
                    REQUIRE( returned2[3] == expected[3]);
                    REQUIRE( returned2[4] == expected[4]);
                }
            }
        }        
    }

PEAKINGDUCK_NAMESPACE_END // unittests
PEAKINGDUCK_NAMESPACE_END // peakingduck