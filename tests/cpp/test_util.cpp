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

    SCENARIO( "Get window - int" ) {
        std::vector<int> values = {45, 2, 78, 23, 23, 89, 27, 23, 67};
        
        int nouter = 1;
        int ninner = 0;
        THEN( "check nouter=1, ninner=0" ) {
            REQUIRE( std::vector<int>{45, 78} ==  util::get_window(values, 1, nouter, ninner, false));
            REQUIRE( std::vector<int>{45, 2, 78} ==  util::get_window(values, 1, nouter, ninner, true));
            REQUIRE( std::vector<int>{2} ==  util::get_window(values, 0, nouter, ninner, false));
            REQUIRE( std::vector<int>{45, 2} ==  util::get_window(values, 0, nouter, ninner, true));
            REQUIRE( std::vector<int>{23} ==  util::get_window(values, 8, nouter, ninner, false));
            REQUIRE( std::vector<int>{23, 67} ==  util::get_window(values, 8, nouter, ninner, true));
        }      
        
        nouter = 0;
        ninner = 0;
        THEN( "check nouter=0, ninner=0" ) {
            REQUIRE( std::vector<int>{} ==  util::get_window(values, 1, nouter, ninner, false));
            REQUIRE( std::vector<int>{2} ==  util::get_window(values, 1, nouter, ninner, true));
            REQUIRE( std::vector<int>{} ==  util::get_window(values, 0, nouter, ninner, false));
            REQUIRE( std::vector<int>{45} ==  util::get_window(values, 0, nouter, ninner, true));
            REQUIRE( std::vector<int>{} ==  util::get_window(values, 8, nouter, ninner, false));
            REQUIRE( std::vector<int>{67} ==  util::get_window(values, 8, nouter, ninner, true));
        }    
        
        nouter = 4;
        ninner = 0;
        THEN( "check nouter=4, ninner=0" ) {
            REQUIRE( std::vector<int>{45, 78, 23, 23, 89} ==  util::get_window(values, 1, nouter, ninner, false));
            REQUIRE( std::vector<int>{45, 2, 78, 23, 23, 89} ==  util::get_window(values, 1, nouter, ninner, true));
            REQUIRE( std::vector<int>{2, 78, 23, 23} ==  util::get_window(values, 0, nouter, ninner, false));
            REQUIRE( std::vector<int>{45, 2, 78, 23, 23} ==  util::get_window(values, 0, nouter, ninner, true));
            REQUIRE( std::vector<int>{23, 89, 27, 23} ==  util::get_window(values, 8, nouter, ninner, false));
            REQUIRE( std::vector<int>{23, 89, 27, 23, 67} ==  util::get_window(values, 8, nouter, ninner, true));
        }      
        
        nouter = 4;
        ninner = 2;
        THEN( "check nouter=4, ninner=2" ) {
            REQUIRE( std::vector<int>{45, 2, 23, 67} ==  util::get_window(values, 4, nouter, ninner, false));
            REQUIRE( std::vector<int>{45, 2, 23, 23, 67} ==  util::get_window(values, 4, nouter, ninner, true));
            REQUIRE( std::vector<int>{23, 23} ==  util::get_window(values, 0, nouter, ninner, false));
            REQUIRE( std::vector<int>{45, 23, 23} ==  util::get_window(values, 0, nouter, ninner, true));
            REQUIRE( std::vector<int>{23, 89} ==  util::get_window(values, 8, nouter, ninner, false));
            REQUIRE( std::vector<int>{23, 89, 67} ==  util::get_window(values, 8, nouter, ninner, true));
        }          
    }

NAMESPACE_END // unittests
NAMESPACE_END // peakingduck