////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////


/*!
    @file
    Defines the constants (mathematical and physical) of the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef UTIL_CONSTANTS_HPP
#define UTIL_CONSTANTS_HPP

#include <units.h>

#include "common.hpp"

NAMESPACE_START(peakingduck)
NAMESPACE_START(constants)

static constexpr const double PI = units::constants::pi;

NAMESPACE_END //constants
NAMESPACE_END //peakingduck

#endif // UTIL_CONSTANTS_HPP