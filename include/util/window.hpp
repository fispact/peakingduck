////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the windowing algorithms/utilities of the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef UTIL_WINDOW_HPP
#define UTIL_WINDOW_HPP

#include <vector>

#include "common.hpp"
#include "constants.hpp"

NAMESPACE_START(peakingduck)
NAMESPACE_START(util)

    template<typename T>
    std::vector<T> get_window(const std::vector<T>& values, 
        int centerindex, int nouter=5, int ninner=0, bool includeindex=true){
            //todo: implement
            return std::vector<T>(values.size(), constants::PI);
    }

NAMESPACE_END
NAMESPACE_END

#endif // UTIL_WINDOW_HPP