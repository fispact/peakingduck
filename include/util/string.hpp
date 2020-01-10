////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the string only utilities of the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef UTIL_STRING_HPP
#define UTIL_STRING_HPP

#include <string>
#include <sstream>
#include <iostream>

#include "common.hpp"

NAMESPACE_START(peakingduck)
NAMESPACE_START(util)

    /*!
     *  Split from a stream using single delimiter
     *  per line
     */
    template <char delimiter, class Container>
    void split(std::istream& stream, Container& cont)
    {
        // get the line from the stream
        std::string line, token;
        std::getline(stream, line);
        
        // now split the line with the delimiter
        std::stringstream ss(line);
        while (std::getline(ss, token, delimiter)) {
            // ignore all whitespace lines
            if(token.find_first_not_of(' ') != std::string::npos){
                cont.push_back(token);
            }
        }
    }

    // trim from start (in place) using whitespace
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }
    // trim from end (in place) using whitespace
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }
    // trim from start (in place) using custom delimiter
    static inline void ltrim(std::string &s, char delimiter) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](char ch) {
            return ch != delimiter;
        }));
    }
    // trim from end (in place) using custom delimiter
    static inline void rtrim(std::string &s, char delimiter) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](char ch) {
            return ch != delimiter;
        }).base(), s.end());
    }

    // trim from both ends (in place) using whitespace
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

NAMESPACE_END //util
NAMESPACE_END //peakingduck

#endif //UTIL_STRING_HPP