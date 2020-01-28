////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the core 1D datastructure (an array) for all manipulation within of the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef CORE_SPECTRAL_HPP
#define CORE_SPECTRAL_HPP

#include <iostream>
#include <vector>

#include "common.hpp"
#include "numerical.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Represents a basic 1D histogram
       Energies vs values or
       Channel vs values
    */
    template<typename XScalar, typename YScalar>
    class Histogram{
        public:

            // only exists for deserialization - maybe this is bad architecture?
            Histogram() :
            _X(NumericalData<XScalar>(0)), _Y(NumericalData<YScalar>(0)) 
            {
            };

            // at construction time we should fix values
            Histogram(const NumericalData<XScalar>& X, const NumericalData<YScalar>& Y) :
            _X(X), _Y(Y) 
            {
                assert(X.size() == (Y.size() +1));
            };

            // remember the rule of 5
            // copy constructor
            Histogram(const Histogram& other) :
            Histogram(other._X, other._Y)
            {}
 
            // // move constructor
            Histogram(Histogram&& other) noexcept :
            Histogram(std::move(other))
            {}

            // copy assignment
            Histogram& operator=(const Histogram& other)
            {
                return *this = Histogram(other);
            }

            // move assignment
            Histogram& operator=(Histogram&& other) noexcept
            {
                std::swap(_X, other._X);
                std::swap(_Y, other._Y);
                return *this;
            }

            // destructor
            virtual ~Histogram() {};

            // need to add operators +,-,*
            // maybe /?
            // ToDo:

            inline NumericalData<XScalar> X() const{
                return _X;
            }

            inline NumericalData<YScalar> Y() const{
                return _Y;
            }

            // need to support rebinning also
            // ToDo:

        protected:
            NumericalData<XScalar> _X;
            NumericalData<YScalar> _Y;
    };

    /*!
       @brief Represents a basic 1D histogram
       Energies vs values or
       Channel vs values
    */
    template<typename XScalar, typename YScalar>
    struct Spectrum : public Histogram<XScalar, YScalar>{

        using Histogram<XScalar, YScalar>::Histogram;

        void removeBackground(int iterations){
            // perform snip on Y values
            this->_Y = this->_Y - this->_Y.snip(iterations);
        }

        NumericalData<YScalar> estimateBackground(int iterations){
            return this->_Y.snip(iterations);
        }
    };

    // serialization methods
    PEAKINGDUCK_NAMESPACE_START(io)

        constexpr char DEFAULTDELIMITER = ' ';

        // template <typename XScalar, typename YScalar, char delimiter=DEFAULTDELIMITER>
        // std::ostream& operator<<(std::ostream& os, const Histogram& hist)

        /*!
             @brief  Deserialization method for histogram

             Assumes delimited text data in form of:

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

PEAKINGDUCK_NAMESPACE_END //core
PEAKINGDUCK_NAMESPACE_END // peakingduck

#endif // CORE_SPECTRAL_HPP