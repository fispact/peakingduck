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

#include <iterator>

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
    template <typename IntegerType,IntegerType ibegin,IntegerType iend,IntegerType step=1>
    struct range {
        struct iterator {
            using value_type=IntegerType;
            using size_type=std::size_t;
            using difference_type=IntegerType;
            using pointer=value_type*;
            using reference=value_type&;
            using iterator_category=std::random_access_iterator_tag;
                
            iterator(IntegerType v) : value(v) 
            {}

            operator IntegerType() const  { return value;}
            operator IntegerType&()       { return value;}

            IntegerType operator*() const { return value;}
            IntegerType const* operator ->() const { return &value; }

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

          private:
            value_type value;
        };

        iterator begin() { return ibegin; }
        iterator end() { return iend; }
        iterator begin() const { return ibegin; }
        iterator end() const { return iend; }
    };

    /*! 
        @brief A simple range based struct.
        Assumes begin, end and step not known at compile time
        Only used for trivial loops to save doing things
        like:
            std::vector<int> indices;
            for(int i=1, i<5; ++i)
                indices.push_back(i);

        Usage as:

            auto rn = range<size_t>(1,5,1);
            for (auto it=rn.begin();it!=rn.end();++it)
                std::cout << *it << ", ";

            1, 2, 3, 4,
    */
    template <typename IntegerType>
    struct rrange {
        rrange(IntegerType ibegin,IntegerType iend,IntegerType step=1) :
        _ibegin(ibegin),
        _iend(iend),
        _step(step)
        {
        }

        struct iterator {
            using value_type=IntegerType;
            using size_type=std::size_t;
            using difference_type=IntegerType;
            using pointer=value_type*;
            using reference=value_type&;
            using iterator_category=std::random_access_iterator_tag;

            iterator(rrange<value_type>& range, IntegerType v) : _range(range), value(v) 
            {}

            operator IntegerType() const  { return value;}
            operator IntegerType&()       { return value;}

            IntegerType operator*() const { return value;}
            IntegerType const* operator ->() const { return &value; }

            iterator& operator++()
            { 
                value += _range._step;
                return *this; 
            }

            iterator& operator++(int)
            { 
                auto copy = *this;
                ++*this;
                return copy;
            }

            bool operator==(iterator const& other) const {
                return _range._step > 0 ? value >= other.value
                                : value < other.value;
            }

            bool operator!=(iterator const& other) const {
                return not (*this == other);
            }
          private:
            rrange<value_type>& _range;
            value_type value;
        };

        iterator begin() { return iterator(*this, _ibegin); }
        iterator end() { return iterator(*this, _iend); }
        iterator begin() const { return iterator(*this, _ibegin); }
        iterator end() const { return iterator(*this, _iend); }

      private:
        IntegerType _ibegin;
        IntegerType _iend;
        const IntegerType _step;
    };

PEAKINGDUCK_NAMESPACE_END //util
PEAKINGDUCK_NAMESPACE_END //peakingduck

#endif //UTIL_STREAM_HPP