////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the macros for the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef COMMON_HPP
#define COMMON_HPP

#ifdef PEAKINGDUCK_NAMESPACE_START
    #undef PEAKINGDUCK_NAMESPACE_START
#endif
#define PEAKINGDUCK_NAMESPACE_START(name) namespace name {

#ifdef PEAKINGDUCK_NAMESPACE_END
    #undef PEAKINGDUCK_NAMESPACE_END
#endif
#define PEAKINGDUCK_NAMESPACE_END }

#ifdef PEAKINGDUCK_NAMESPACE_USING
    #undef PEAKINGDUCK_NAMESPACE_USING
#endif
#define PEAKINGDUCK_NAMESPACE_USING(name) using namespace name;

constexpr char NEWLINE = '\n';

#endif // COMMON_HPP
