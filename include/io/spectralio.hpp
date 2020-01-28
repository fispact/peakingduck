////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the I/O for spectral data.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef IO_SPECTRAL_HPP
#define IO_SPECTRAL_HPP

#include <iostream>
#include <vector>

#include "common.hpp"
#include "util/string.hpp"
#include "core/spectral.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(io)

    constexpr char DEFAULTDELIMITER = ' ';

    // template <typename XScalar, typename YScalar, char delimiter=DEFAULTDELIMITER>
    // std::ostream& operator<<(std::ostream& os, const Histogram& hist)

    /*!
            @brief  Deserialization method for histogram

            Assumes delimited text data in column form of:

            channel,lowerenergy,upperenergy,count         
    */
    template <typename XScalar, typename YScalar, char delimiter>
    static void Deserialize(std::istream& stream, core::Histogram<XScalar, YScalar>& hist)
    {
        // get the headers
        std::vector<std::string> row;
        util::split<delimiter>(stream, row);

        // we expect 4 columns
        // can we do this at compile time too?
        // use std::tuple instead?
        assert(row.size() == 4);
        
        // iterate until the end of the file
        // all stored as double values in file
        row.resize(0);
        util::split<delimiter>(stream, row);

        std::vector<XScalar> X;
        std::vector<YScalar> Y;

        X.push_back(static_cast<XScalar>(std::stod(row[1])));
        while(row.size()>0){
            X.push_back(static_cast<XScalar>(std::stod(row[2])));
            Y.push_back(static_cast<YScalar>(std::stod(row[3])));
            row.resize(0);

            // get next line
            util::split<delimiter>(stream, row);
        }

        // convert to histogram
        hist = core::Histogram<XScalar, YScalar>(core::NumericalData<XScalar>(X), core::NumericalData<YScalar>(Y));
    }

    template <typename XScalar, typename YScalar, char delimiter=DEFAULTDELIMITER>
    std::istream& operator>>(std::istream& is, core::Histogram<XScalar, YScalar>& hist)
    {
        Deserialize<XScalar, YScalar, delimiter>(is, hist);
        return is;
    }

PEAKINGDUCK_NAMESPACE_END // io
PEAKINGDUCK_NAMESPACE_END // peakingduck

#endif // IO_SPECTRAL_HPP