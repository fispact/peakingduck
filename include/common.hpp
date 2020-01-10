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

#ifdef NAMESPACE_START
    #undef NAMESPACE_START
#endif
#define NAMESPACE_START(name) namespace name {

#ifdef NAMESPACE_END
    #undef NAMESPACE_END
#endif
#define NAMESPACE_END }

#ifdef NAMESPACE_USING
    #undef NAMESPACE_USING
#endif
#define NAMESPACE_USING(name) using namespace name;

constexpr char NEWLINE = '\n';

#endif // COMMON_HPP
