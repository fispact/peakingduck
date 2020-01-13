////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the base class for the Curiously Recurring Template Pattern (CRTP).

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef CORE_CRTP_HPP
#define CORE_CRTP_HPP

#include "common.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(core)

    /*!
       @brief Represents the base CRTP class to allow objects 
       to have certain abilities in their interface
    */
    template <typename T, template<typename> class crtpType>
    class crtp
    {
        public:
            T& underlying() 
            { 
                return static_cast<T&>(*this); 
            }

            T const& underlying() const 
            { 
                return static_cast<T const&>(*this); 
            }
            
        private:
            crtp(){}
            friend crtpType<T>;
    };

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // CORE_CRTP_HPP