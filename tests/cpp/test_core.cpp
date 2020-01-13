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
    }

PEAKINGDUCK_NAMESPACE_END // unittests
PEAKINGDUCK_NAMESPACE_END // peakingduck