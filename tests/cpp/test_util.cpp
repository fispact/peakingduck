////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "catch.hpp"

#include "common.hpp"

#include "peakingduck.hpp"

NAMESPACE_START(peakingduck)
NAMESPACE_START(unittests)

    SCENARIO( "Split a string by space" ) {
        // create a string for splitting
        std::stringstream stream("split me into a vector");
        std::vector<std::string> contents;

        util::split<' '>(stream, contents);
        
        int index = 0;
        THEN( "check split" ) {
            REQUIRE( contents.size() == 5);
            REQUIRE( contents[index++] == "split");
            REQUIRE( contents[index++] == "me");
            REQUIRE( contents[index++] == "into");
            REQUIRE( contents[index++] == "a");
            REQUIRE( contents[index++] == "vector");
        }        
    }

    SCENARIO( "Split a string by comma" ) {
        // create a string for splitting
        std::stringstream stream("split,me,into,a vector,ignore,that,last,one");
        std::vector<std::string> contents;

        util::split<','>(stream, contents);
        
        int index = 0;
        THEN( "check split" ) {
            REQUIRE( contents.size() == 8);
            REQUIRE( contents[index++] == "split");
            REQUIRE( contents[index++] == "me");
            REQUIRE( contents[index++] == "into");
            REQUIRE( contents[index++] == "a vector");
            REQUIRE( contents[index++] == "ignore");
            REQUIRE( contents[index++] == "that");
            REQUIRE( contents[index++] == "last");
            REQUIRE( contents[index++] == "one");
        }        
    }

NAMESPACE_END // peakingduck
NAMESPACE_END // inputtests