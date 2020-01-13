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
        core::NumericalData data(4);

        data << 1,2,3,4;
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