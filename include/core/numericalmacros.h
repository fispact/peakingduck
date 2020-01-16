////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the core numerical macros to reduce copy pasting of wrapping Eigen operators 

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/

#ifndef CORE_NUMERICAL_MACROS_H
#define CORE_NUMERICAL_MACROS_H

#define PEAKINGDUCK_NUMERICAL_OPERATOR_IMP_MACRO(NUMERICAL_TYPE, BASE_NUMERICAL_TYPE, OP)                       \
inline NUMERICAL_TYPE operator OP(const T& scalar) const                                                        \
{                                                                                                               \
    return this->BASE_NUMERICAL_TYPE::operator OP(scalar);                                                      \
}                                                                                                               \
friend inline NUMERICAL_TYPE operator OP (const T& scalar, const NUMERICAL_TYPE & rhs)                          \
{                                                                                                               \
    return scalar OP static_cast<const BASE_NUMERICAL_TYPE &>(rhs);                                             \
}                                                                                                               \
inline NUMERICAL_TYPE operator OP(const NUMERICAL_TYPE & rhs) const                                             \
{                                                                                                               \
    return static_cast<const BASE_NUMERICAL_TYPE &>(*this) OP static_cast<const BASE_NUMERICAL_TYPE &>(rhs);    \
}                                                                                                                \
inline const NUMERICAL_TYPE & operator OP ## =(const NUMERICAL_TYPE & rhs)                                      \
{                                                                                                               \
    this->BASE_NUMERICAL_TYPE::operator OP ## =(static_cast<const BASE_NUMERICAL_TYPE&>(rhs));                  \
    return *this;                                                                                               \
}     

#endif //CORE_NUMERICAL_MACROS_H