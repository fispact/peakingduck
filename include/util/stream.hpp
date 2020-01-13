////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the stream only utilities of the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef UTIL_STREAM_HPP
#define UTIL_STREAM_HPP

#include <string>
#include <sstream>

#include "common.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(util)
    
    /*!
    * A function to read a istream as a string
    * @brief Will return a string from a buffered stream
    * @throws ios::failure if cannot read the buffer
    * @param instream the istream buffer
    * @return A string containing the whole buffer
    */
    std::string read_stream_into_string(std::istream& instream){
        auto ss = std::ostringstream{};
        
        if(!(ss << instream.rdbuf()))
            throw std::ios::failure{"Error reading buffer"};
        
        return ss.str();
    }
    
PEAKINGDUCK_NAMESPACE_END //util
PEAKINGDUCK_NAMESPACE_END //peakingduck

#endif //UTIL_STREAM_HPP
