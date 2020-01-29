////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "catch.hpp"

#include "common.hpp"

#include "peakingduck.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(unittests)

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

    SCENARIO( "Trim a string by whitespace" ) {
        std::string teststring("    some blanks at start and end    .     ");
        
        THEN( "check ltrim" ) {
            util::ltrim(teststring);
            REQUIRE( teststring == "some blanks at start and end    .     ");
        }   
        THEN( "check rtrim" ) {
            util::rtrim(teststring);
            REQUIRE( teststring == "    some blanks at start and end    .");
        }     
        THEN( "check lrtrim" ) {
            util::trim(teststring);
            REQUIRE( teststring == "some blanks at start and end    .");
        }             
    }

    SCENARIO( "Trim a string by custom" ) {        
        THEN( "check ltrim" ) {
            std::string teststring("...  some blanks at start and end    .     ");
            util::ltrim(teststring, '.');
            REQUIRE( teststring == "  some blanks at start and end    .     ");
        }   
        THEN( "check rtrim" ) {
            std::string teststring("remove trailing T's TTTTTTTTT");
            util::rtrim(teststring, 'T');
            REQUIRE( teststring == "remove trailing T's ");
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

    SCENARIO( "Using custom range" ) {        
        THEN( "check simple" ) {
            auto rn = util::range<size_t, 0, 5, 1>();
            std::vector<int> vec;
            for(auto it=rn.begin(); it!=rn.end(); ++it){
                vec.push_back(it);
            }
            REQUIRE( std::vector<int>{0, 1, 2, 3, 4} == vec);
        }             
        THEN( "check simple non zero start" ) {
            auto rn = util::range<size_t, 10, 20, 1>();
            std::vector<int> vec;
            for(auto it=rn.begin(); it!=rn.end(); ++it){
                vec.push_back(it);
            }
            REQUIRE( std::vector<int>{10, 11, 12, 13, 14, 15, 16, 17, 18, 19} == vec);
        }               
        THEN( "check simple non zero start and different step" ) {
            auto rn = util::range<int, 10, 20, 3>();
            std::vector<int> vec;
            for(auto it=rn.begin(); it!=rn.end(); ++it){
                vec.push_back(it);
            }
            REQUIRE( std::vector<int>{10, 13, 16, 19} == vec);
        }           
        THEN( "check reverse" ) {
            auto rn = util::range<int, 20, 15, -1>();
            std::vector<int> vec;
            for(auto it=rn.begin(); it!=rn.end(); ++it){
                vec.push_back(it);
            }
            REQUIRE( std::vector<int>{20, 19, 18, 17, 16, 15} == vec);
        }         
        THEN( "check reverse and different step" ) {
            auto rn = util::range<int, 20, 0, -5>();
            std::vector<int> vec;
            for(auto it=rn.begin(); it!=rn.end(); ++it){
                vec.push_back(it);
            }
            REQUIRE( std::vector<int>{20, 15, 10, 5, 0} == vec);
        }            
    }

PEAKINGDUCK_NAMESPACE_END // unittests
PEAKINGDUCK_NAMESPACE_END // peakingduck