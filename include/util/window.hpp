////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines the windowing algorithms/utilities of the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef UTIL_WINDOW_HPP
#define UTIL_WINDOW_HPP

#include <cassert>
#include <vector>

#include "common.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(util)

    /*!
       @brief Given a list of values take nouter points either side of 
        the index given and ignore ninner points.

            Examples:

            1. 
                values = [8, 2, 5, 2, 6, 6, 9, 23, 12]
                index = 4
                nouter = 3
                ninner = 0
                includeindex = True

                => [2, 5, 2, 6, 6, 9, 23]
            2. 
                values = [8, 2, 5, 2, 6, 6, 9, 23, 12]
                index = 4
                nouter = 3
                ninner = 0
                includeindex = False

                => [2, 5, 2, 6, 9, 23]
            3. 
                values = [8, 2, 5, 2, 6, 6, 9, 23, 12]
                index = 4
                nouter = 3
                ninner = 1
                includeindex = True

                => [2, 5, 6, 9, 23]
            4. 
                values = [8, 2, 5, 2, 6, 6, 9, 23, 12]
                index = 4
                nouter = 3
                ninner = 1
                includeindex = False

                => [2, 5, 9, 23]

        Therefore:
            - ninner >= 0
            - ninner <= nouter
            - index >= nouter
            - index < values.size()

        It will clip at (0, len(values))

    */
    template<typename T>
    static std::vector<T> get_window(const std::vector<T>& values, 
        int centerindex, int nouter=5, int ninner=0, bool includeindex=true){

        // no funny business
        assert(ninner <= nouter);
        assert((centerindex >= 0) && (centerindex < static_cast<int>(values.size())));
        assert(values.size() > 0);

        const auto lowerpart1 = values.begin() + std::max(0, centerindex-nouter);
        const auto lowerpart2 = values.begin() + std::max(0, centerindex-ninner);
        const auto upperpart1 = values.begin() + std::min(static_cast<int>(values.size()), centerindex+1+ninner);
        const auto upperpart2 = values.begin() + std::min(static_cast<int>(values.size()), centerindex+1+nouter);

        std::vector<T> resultvec(lowerpart1, lowerpart2);

        if(includeindex){
            resultvec.insert(std::end(resultvec), values[centerindex]);
        }
        resultvec.insert(std::end(resultvec), upperpart1, upperpart2);
        return resultvec;
    }

PEAKINGDUCK_NAMESPACE_END
PEAKINGDUCK_NAMESPACE_END

#endif // UTIL_WINDOW_HPP
