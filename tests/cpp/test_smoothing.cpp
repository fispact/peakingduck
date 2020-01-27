////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

#include <vector>

#include "catch.hpp"

#include "common.hpp"

#include "peakingduck.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(unittests)

    template<typename T=double, int Size=core::ArrayTypeDynamic>
    void REQUIRE_NUMERICS_APPROX_THE_SAME(const core::NumericalData<T, Size>& lhs, const core::NumericalData<T, Size>& rhs){
        REQUIRE( lhs.size() == rhs.size());
        for(int i=0;i<lhs.size();++i){
            REQUIRE( lhs[i] == Approx(rhs[i]) );
        }
    }

    SCENARIO( "Test moving average smoother" ) {
        const core::NumericalData<double, 11> data(3, 5, 4, 12, 23, 3, 7, 5, 3, 4, 8);
        using ND = core::NumericalData<double, 11>;
        using SMOOTHER = core::MovingAverageSmoother<double, 11>;
        THEN("window 1"){
            const ND smoothed = SMOOTHER(1)(data);
            const ND expected(3.0, 4.0, 7.0, 13.0, 38./3., 11.0, 5.0, 5.0, 4.0, 5.0, 8.0);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, smoothed);
        }
        THEN("window 2"){
            const ND smoothed = SMOOTHER(2)(data);
            const ND expected(3.0, 5.0, 9.4, 9.4, 9.8, 10.0, 8.2, 4.4, 5.4, 4.0, 8.0);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, smoothed);
        }
        THEN("window 3"){
            const ND smoothed = SMOOTHER(3)(data);
            const ND expected(3.0, 5.0, 4.0, 57./7.0, 59./7.0, 57./7.0, 57./7.0, 53./7.0, 3.0, 4.0, 8.0);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, smoothed);
        }
    }


PEAKINGDUCK_NAMESPACE_END // unittests
PEAKINGDUCK_NAMESPACE_END // peakingduck
