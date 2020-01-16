////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

#include "catch.hpp"

#include "common.hpp"

#include "peakingduck.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(unittests)

    SCENARIO( "Test numerical array" ) {
        // create a string for splitting
        core::NumericalData<double, 4> data({1,2,3,4});

        // data << 1,2,3,4;
        THEN( "check scale" ) {
            data.scale(4);
            REQUIRE( data[0] == 4);
            REQUIRE( data[1] == 8);
            REQUIRE( data[2] == 12);
            REQUIRE( data[3] == 16);

            THEN( "change last entry and scale again" ) {
                data[3] = 20;
                data.scale(2);
                REQUIRE( data[0] == 8);
                REQUIRE( data[1] == 16);
                REQUIRE( data[2] == 24);
                REQUIRE( data[3] == 40);
            }
        } 

        THEN( "check + operations" ) {
            THEN( "check += scalar" ) {
                data += 3.0;
                REQUIRE( data[0] == 4);
                REQUIRE( data[1] == 5);
                REQUIRE( data[2] == 6);
                REQUIRE( data[3] == 7);
            }
            THEN( "check += array" ) {
                data += core::NumericalData<double, 4>({1, 2, 3, 5});
                REQUIRE( data[0] == 2);
                REQUIRE( data[1] == 4);
                REQUIRE( data[2] == 6);
                REQUIRE( data[3] == 9);
            }
            THEN( "check += array dynamic" ) {
                data += (core::NumericalData<double>(4) << 1, 2, 3, 5).finished();
                REQUIRE( data[0] == 2);
                REQUIRE( data[1] == 4);
                REQUIRE( data[2] == 6);
                REQUIRE( data[3] == 9);
            }
            THEN( "check sum of array + scalar" ) {
                auto coeffSum = data + 6.0;
                REQUIRE( coeffSum[0] == 7);
                REQUIRE( coeffSum[1] == 8);
                REQUIRE( coeffSum[2] == 9);
                REQUIRE( coeffSum[3] == 10);
            }
            THEN( "check sum of two arrays" ) {
                auto coeffSum = data + data;
                REQUIRE( coeffSum[0] == 2);
                REQUIRE( coeffSum[1] == 4);
                REQUIRE( coeffSum[2] == 6);
                REQUIRE( coeffSum[3] == 8);
            }
            THEN( "check sum of two vectors and scaled" ) {
                auto coeffSum = data + data*2.0;
                REQUIRE( coeffSum[0] == 3);
                REQUIRE( coeffSum[1] == 6);
                REQUIRE( coeffSum[2] == 9);
                REQUIRE( coeffSum[3] == 12);
            }
            THEN( "check sum of two vectors and scaled lhs" ) {
                auto coeffSum = data + 2.0*data;
                REQUIRE( coeffSum[0] == 3);
                REQUIRE( coeffSum[1] == 6);
                REQUIRE( coeffSum[2] == 9);
                REQUIRE( coeffSum[3] == 12);
            }
        }  

        THEN( "check - operations" ) {
            THEN( "check -= scalar" ) {
                data -= 1.0;
                REQUIRE( data[0] == 0);
                REQUIRE( data[1] == 1);
                REQUIRE( data[2] == 2);
                REQUIRE( data[3] == 3);
            }
            THEN( "check -= array" ) {
                data -= core::NumericalData<double, 4>({1, 2, 3, 5});
                REQUIRE( data[0] == 0);
                REQUIRE( data[1] == 0);
                REQUIRE( data[2] == 0);
                REQUIRE( data[3] == -1);
            }
            THEN( "check -= array dynamic" ) {
                data -= (core::NumericalData<double>(4) << -4, 2, 1, 5).finished();
                REQUIRE( data[0] == 5);
                REQUIRE( data[1] == 0);
                REQUIRE( data[2] == 2);
                REQUIRE( data[3] == -1);
            }
            THEN( "check array - scalar" ) {
                auto coeffSum = data - 6.0;
                REQUIRE( coeffSum[0] == -5);
                REQUIRE( coeffSum[1] == -4);
                REQUIRE( coeffSum[2] == -3);
                REQUIRE( coeffSum[3] == -2);
            }
            THEN( "check scalar - array" ) {
                auto coeffSum = 10.0 - data;
                REQUIRE( coeffSum[0] == 9);
                REQUIRE( coeffSum[1] == 8);
                REQUIRE( coeffSum[2] == 7);
                REQUIRE( coeffSum[3] == 6);
            }
            THEN( "check difference of two arrays" ) {
                auto coeffSum = data - data;
                REQUIRE( coeffSum[0] == 0);
                REQUIRE( coeffSum[1] == 0);
                REQUIRE( coeffSum[2] == 0);
                REQUIRE( coeffSum[3] == 0);
            }
            THEN( "check difference of two vectors and scaled" ) {
                auto coeffSum = data - data*2.0;
                REQUIRE( coeffSum[0] == -1);
                REQUIRE( coeffSum[1] == -2);
                REQUIRE( coeffSum[2] == -3);
                REQUIRE( coeffSum[3] == -4);
            }
            THEN( "check difference of two vectors and scaled lhs" ) {
                auto coeffSum = data - 2.0*data;
                REQUIRE( coeffSum[0] == -1);
                REQUIRE( coeffSum[1] == -2);
                REQUIRE( coeffSum[2] == -3);
                REQUIRE( coeffSum[3] == -4);
            }
        }  

        THEN( "check map methods" ) {
            THEN( "reverse" ) {
                core::Array1Dd reverse = data.reverse();
                REQUIRE( reverse[0] == 4.0);
                REQUIRE( reverse[1] == 3.0);
                REQUIRE( reverse[2] == 2.0);
                REQUIRE( reverse[3] == 1.0);
            }  
            THEN( "reverse in place" ) {
                data.reverseInPlace();
                REQUIRE( data[0] == 4.0);
                REQUIRE( data[1] == 3.0);
                REQUIRE( data[2] == 2.0);
                REQUIRE( data[3] == 1.0);
            }  
            THEN( "square" ) {
                core::Array1Dd squared = data.square();
                REQUIRE( squared[0] == 1.0);
                REQUIRE( squared[1] == 4.0);
                REQUIRE( squared[2] == 9.0);
                REQUIRE( squared[3] == 16.0);
            }  
            THEN( "sqrt" ) {
                data << 100.0, 4.0, 9.0, 16.0;
                core::Array1Dd sqrted = data.sqrt();
                REQUIRE( sqrted[0] == 10.0);
                REQUIRE( sqrted[1] == 2.0);
                REQUIRE( sqrted[2] == 3.0);
                REQUIRE( sqrted[3] == 4.0);
            }  
            THEN( "pow" ) {
                data << 1,2,3,4;
                core::Array1Dd sqrted = data.pow(3);
                REQUIRE( sqrted[0] == 1.0);
                REQUIRE( sqrted[1] == 8.0);
                REQUIRE( sqrted[2] == 27.0);
                REQUIRE( sqrted[3] == 64.0);
            }  
        }  

        THEN( "check reduce methods" ) {
            REQUIRE( data.mean() == 2.5);
            REQUIRE( data.sum() == 10.0);
            REQUIRE( data.minCoeff() == 1.0);
            REQUIRE( data.maxCoeff() == 4.0);
        }  

        THEN( "check all operations" ) {
            REQUIRE( (data > 0).all() == 1);
            REQUIRE( (data < 5).all() == 1);
            REQUIRE( (data < 3).all() == 0);
            REQUIRE( (data == 1).all() == 0);

            data << 1,1,1,1;
            REQUIRE( (data > 0).all() == 1);
            REQUIRE( (data < 5).all() == 1);
            REQUIRE( (data < 3).all() == 1);
            REQUIRE( (data == 1).all() == 1);
        }          

        THEN( "check any operations" ) {
            REQUIRE( (data > 0).any() == 1);
            REQUIRE( (data < 5).any() == 1);
            REQUIRE( (data < 3).any() == 1);
            REQUIRE( (data == 1).any() == 1);
            REQUIRE( (data == 5).any() == 0);
        }     
           
        THEN( "check custom ramp method" ) {
            data.ramp(3);
            REQUIRE( data[0] == 0);
            REQUIRE( data[1] == 0);
            REQUIRE( data[2] == 3);
            REQUIRE( data[3] == 4);
        }    
    }

PEAKINGDUCK_NAMESPACE_END // unittests
PEAKINGDUCK_NAMESPACE_END // peakingduck