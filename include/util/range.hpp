////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines a simlpe range based struct. For use in loops.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef UTIL_RANGE_HPP
#define UTIL_RANGE_HPP

#include "common.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(util)

    /*! 
        @brief A simple range based struct.
        Assumes begin, end and step known at compile time
        Only used for trivial loops to save doing things
        like:
            std::vector<int> indices;
            for(int i=1, i<5; ++i)
                indices.push_back(i);

        Usage as:

            auto rn = range<size_t,1,5,1>();
            for (auto it=rn.begin();it!=rn.end();++it)
                std::cout << *it << ", ";

            1, 2, 3, 4,
    */
    template <typename T,T ibegin,T iend,T step=1>
    struct range {
        struct iterator {
            T value;

            iterator(T v) : value(v) 
            {}

            operator T() const  { return value;}
            operator T&()       { return value;}

            T operator*() const { return value;}
            T const* operator ->() const { return &value; }

            iterator& operator++()
            { 
                value += step;
                return *this; 
            }

            iterator& operator++(int)
            { 
                auto copy = *this;
                ++*this;
                return copy;
            }

            bool operator==(iterator const& other) const {
                return step > 0 ? value >= other.value
                                : value < other.value;
            }

            bool operator!=(iterator const& other) const {
                return not (*this == other);
            }
        };

    iterator begin() { return ibegin; }
    iterator end() { return iend; }
    };

PEAKINGDUCK_NAMESPACE_END //util
PEAKINGDUCK_NAMESPACE_END //peakingduck

#endif //UTIL_STREAM_HPP