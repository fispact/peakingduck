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

#include "common.hpp"
#include "core/numerical.hpp"

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
 
            // move constructor
            Histogram(Histogram&& other) = default;

            // copy assignment
            Histogram& operator=(const Histogram& other) = default;

            // move assignment
            Histogram& operator=(Histogram&& other) noexcept = default;

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

        template<class Iterator>
        void removeBackground(Iterator first, Iterator last){
            // perform snip on Y values
            this->_Y = this->_Y - this->_Y.snip(first, last);
        }

        template<class Iterator>
        NumericalData<YScalar> estimateBackground(Iterator first, Iterator last) const{
            return this->_Y.snip(first, last);
        }
    };

PEAKINGDUCK_NAMESPACE_END //core
PEAKINGDUCK_NAMESPACE_END // peakingduck

#endif // CORE_SPECTRAL_HPP