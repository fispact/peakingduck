////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <vector>

#include "catch.hpp"

#include "common.hpp"

#include "peakingduck.hpp"

PEAKINGDUCK_NAMESPACE_START(peakingduck)
PEAKINGDUCK_NAMESPACE_START(unittests)

    template<typename T=int, int Size=core::ArrayTypeDynamic>
    void REQUIRE_NUMERICS_THE_SAME(const core::NumericalData<T, Size>& lhs, const core::NumericalData<T, Size>& rhs){
        REQUIRE( lhs.size() == rhs.size());
        for(int i=0;i<lhs.size();++i){
            REQUIRE( lhs[i] == rhs[i] );
        }
    }

    template<typename T=double, int Size=core::ArrayTypeDynamic>
    void REQUIRE_NUMERICS_APPROX_THE_SAME(const core::NumericalData<T, Size>& lhs, const core::NumericalData<T, Size>& rhs){
        REQUIRE( lhs.size() == rhs.size());
        for(int i=0;i<lhs.size();++i){
            REQUIRE( lhs[i] == Approx(rhs[i]) );
        }
    }

    /*
        Ideally it would be nice if we could do all this at compile time,
        but as I understand Eigen does not provide constexpr for any
        methods (maybe I am mistaken). 
        If we can do this, then all REQUIRES can be replaced with
        STATIC_REQUIRES.
        I think most modern compilers are quite clever at doing
        some stuff at compile time, let's not waste too much time on 
        this now.
    */
    SCENARIO( "Test numerical array" ) {
        // create a string for splitting
        core::NumericalData<double, 4> data(1,2,3,4);
        REQUIRE( data.to_vector() == std::vector<double>({1,2,3,4}));

        THEN( "check slice" ) {
            THEN( "check slice 1" ) {
                const core::NumericalData<double> slice = data.slice(1, 3);
                REQUIRE( slice.size() == 2);
                REQUIRE( slice[0] == 2);
                REQUIRE( slice[1] == 3);
            }
            THEN( "check slice 2" ) {
                const core::NumericalData<double, 9> data2(1, 4, 5, 2, 10, 2, 2, -8, 2);
                const core::NumericalData<double> slice = data2.slice(1, 4);
                REQUIRE( slice.size() == 3);
                REQUIRE( slice[0] == 4);
                REQUIRE( slice[1] == 5);
                REQUIRE( slice[2] == 2);
            }
            THEN( "check slice 3 - negative" ) {
                const core::NumericalData<double, 9> data2(1, 4, 5, 2, 10, -2, 2, -8, 2);
                const core::NumericalData<double> slice = data2.slice(3, -2);
                REQUIRE( slice.size() == 4);
                REQUIRE( slice[0] == 2);
                REQUIRE( slice[1] == 10);
                REQUIRE( slice[2] == -2);
                REQUIRE( slice[3] == 2);
            }
            THEN( "check slice 4 - comma operator" ) {
                const core::NumericalData<double, 9> data2(1, 4, 5, 2, 10, -2, 2, -8, 2);
                const core::NumericalData<double> slice = data2(3, -2);
                REQUIRE( slice.size() == 4);
                REQUIRE( slice[0] == 2);
                REQUIRE( slice[1] == 10);
                REQUIRE( slice[2] == -2);
                REQUIRE( slice[3] == 2);
            }
            THEN( "check combining slices" ) {
                const core::NumericalData<double, 9> data2(1, 4, 5, 2, 10, -2, 2, -8, 2);
                const core::NumericalData<double> combined = core::combine(data2(1, 4), data2(5, -1));
                REQUIRE( combined.size() == 6);
                REQUIRE( combined[0] == 4);
                REQUIRE( combined[1] == 5);
                REQUIRE( combined[2] == 2);
                REQUIRE( combined[3] == -2);
                REQUIRE( combined[4] == 2);
                REQUIRE( combined[5] == -8);
            }
            THEN( "check combining 3 slices" ) {
                const core::NumericalData<double, 9> data2(1, 4, 5, 2, 10, -2, 2, -8, 2);
                const core::NumericalData<double> combined = core::combine(data2(1, 4), core::combine(data2(4, 5), data2(5, -1)));
                REQUIRE( combined.size() == 7);
                REQUIRE( combined[0] == 4);
                REQUIRE( combined[1] == 5);
                REQUIRE( combined[2] == 2);
                REQUIRE( combined[3] == 10);
                REQUIRE( combined[4] == -2);
                REQUIRE( combined[5] == 2);
                REQUIRE( combined[6] == -8);
            }
        }

        THEN( "check + operations" ) {
            THEN( "check += scalar" ) {
                data += 3.0;
                REQUIRE( data[0] == 4);
                REQUIRE( data[1] == 5);
                REQUIRE( data[2] == 6);
                REQUIRE( data[3] == 7);
            }
            THEN( "check += array" ) {
                data += core::NumericalData<double, 4>(std::vector<double>({1, 2, 3, 5}));
                REQUIRE( data[0] == 2);
                REQUIRE( data[1] == 4);
                REQUIRE( data[2] == 6);
                REQUIRE( data[3] == 9);
            }
            THEN( "check += array dynamic" ) {
                data += (core::NumericalData<double>(4) << 1, 2, 3, 5).finished();
                REQUIRE( data[0] == 2);
                REQUIRE( data[1] == 4);
                REQUIRE( data[2] == 6);
                REQUIRE( data[3] == 9);
            }
            THEN( "check sum of array + scalar" ) {
                auto coeffSum = data + 6.0;
                REQUIRE( coeffSum[0] == 7);
                REQUIRE( coeffSum[1] == 8);
                REQUIRE( coeffSum[2] == 9);
                REQUIRE( coeffSum[3] == 10);
            }
            THEN( "check sum of two arrays" ) {
                auto coeffSum = data + data;
                REQUIRE( coeffSum[0] == 2);
                REQUIRE( coeffSum[1] == 4);
                REQUIRE( coeffSum[2] == 6);
                REQUIRE( coeffSum[3] == 8);
            }
            THEN( "check sum of two vectors and scaled" ) {
                auto coeffSum = data + data*2.0;
                REQUIRE( coeffSum[0] == 3);
                REQUIRE( coeffSum[1] == 6);
                REQUIRE( coeffSum[2] == 9);
                REQUIRE( coeffSum[3] == 12);
            }
            THEN( "check sum of two vectors and scaled lhs" ) {
                auto coeffSum = data + 2.0*data;
                REQUIRE( coeffSum[0] == 3);
                REQUIRE( coeffSum[1] == 6);
                REQUIRE( coeffSum[2] == 9);
                REQUIRE( coeffSum[3] == 12);
            }
        }  

        THEN( "check - operations" ) {
            THEN( "check -= scalar" ) {
                data -= 1.0;
                REQUIRE( data[0] == 0);
                REQUIRE( data[1] == 1);
                REQUIRE( data[2] == 2);
                REQUIRE( data[3] == 3);
            }
            THEN( "check -= array" ) {
                data -= core::NumericalData<double, 4>(1, 2, 3, 5);
                REQUIRE( data[0] == 0);
                REQUIRE( data[1] == 0);
                REQUIRE( data[2] == 0);
                REQUIRE( data[3] == -1);
            }
            THEN( "check -= array dynamic" ) {
                data -= (core::NumericalData<double>(4) << -4, 2, 1, 5).finished();
                REQUIRE( data[0] == 5);
                REQUIRE( data[1] == 0);
                REQUIRE( data[2] == 2);
                REQUIRE( data[3] == -1);
            }
            THEN( "check array - scalar" ) {
                auto coeffSum = data - 6.0;
                REQUIRE( coeffSum[0] == -5);
                REQUIRE( coeffSum[1] == -4);
                REQUIRE( coeffSum[2] == -3);
                REQUIRE( coeffSum[3] == -2);
            }
            THEN( "check scalar - array" ) {
                auto coeffSum = 10.0 - data;
                REQUIRE( coeffSum[0] == 9);
                REQUIRE( coeffSum[1] == 8);
                REQUIRE( coeffSum[2] == 7);
                REQUIRE( coeffSum[3] == 6);
            }
            THEN( "check difference of two arrays" ) {
                auto coeffSum = data - data;
                REQUIRE( coeffSum[0] == 0);
                REQUIRE( coeffSum[1] == 0);
                REQUIRE( coeffSum[2] == 0);
                REQUIRE( coeffSum[3] == 0);
            }
            THEN( "check difference of two vectors and scaled" ) {
                auto coeffSum = data - data*2.0;
                REQUIRE( coeffSum[0] == -1);
                REQUIRE( coeffSum[1] == -2);
                REQUIRE( coeffSum[2] == -3);
                REQUIRE( coeffSum[3] == -4);
            }
            THEN( "check difference of two vectors and scaled lhs" ) {
                auto coeffSum = data - 2.0*data;
                REQUIRE( coeffSum[0] == -1);
                REQUIRE( coeffSum[1] == -2);
                REQUIRE( coeffSum[2] == -3);
                REQUIRE( coeffSum[3] == -4);
            }
        }  

        THEN( "check * operations" ) {
            THEN( "check *= scalar" ) {
                data *= 5.0;
                REQUIRE( data[0] == 5.);
                REQUIRE( data[1] == 10.);
                REQUIRE( data[2] == 15.0);
                REQUIRE( data[3] == 20.);
            }
            THEN( "check *= array" ) {
                data *= core::NumericalData<double, 4>(1, 2, 3, 5);
                REQUIRE( data[0] == 1.);
                REQUIRE( data[1] == 4.);
                REQUIRE( data[2] == 9.);
                REQUIRE( data[3] == 20.);
            }
            THEN( "check *= array dynamic" ) {
                data *= (core::NumericalData<double>(4) << -4, 2, 1, 5).finished();
                REQUIRE( data[0] == -4.);
                REQUIRE( data[1] == 4.);
                REQUIRE( data[2] == 3.);
                REQUIRE( data[3] == 20.);
            }
            THEN( "check array * scalar" ) {
                auto coeffSum = data*6.0;
                REQUIRE( coeffSum[0] == 6.0);
                REQUIRE( coeffSum[1] == 12.0);
                REQUIRE( coeffSum[2] == 18.0);
                REQUIRE( coeffSum[3] == 24.0);
            }
            THEN( "check scalar * array" ) {
                auto coeffSum = 6.0*data;
                REQUIRE( coeffSum[0] == 6.0);
                REQUIRE( coeffSum[1] == 12.0);
                REQUIRE( coeffSum[2] == 18.0);
                REQUIRE( coeffSum[3] == 24.0);
            }
            THEN( "check product of two arrays" ) {
                auto coeffSum = data * data;
                REQUIRE( coeffSum[0] == 1);
                REQUIRE( coeffSum[1] == 4.);
                REQUIRE( coeffSum[2] == 9.);
                REQUIRE( coeffSum[3] == 16.);
            }
            THEN( "check product of two arrays and scalar" ) {
                auto coeffSum = data*data*2.0;
                REQUIRE( coeffSum[0] == 2);
                REQUIRE( coeffSum[1] == 8.);
                REQUIRE( coeffSum[2] == 18.);
                REQUIRE( coeffSum[3] == 32.);
            }
            THEN( "check product of two arrays and scalar 2" ) {
                auto coeffSum = data*2.0*data;
                REQUIRE( coeffSum[0] == 2);
                REQUIRE( coeffSum[1] == 8.);
                REQUIRE( coeffSum[2] == 18.);
                REQUIRE( coeffSum[3] == 32.);
            }
            THEN( "check product of two arrays and scalar 3" ) {
                auto coeffSum = 2.0*data*data;
                REQUIRE( coeffSum[0] == 2);
                REQUIRE( coeffSum[1] == 8.);
                REQUIRE( coeffSum[2] == 18.);
                REQUIRE( coeffSum[3] == 32.);
            }
        }

        THEN( "check / operations" ) {
            THEN( "check /= scalar" ) {
                data /= 2.0;
                REQUIRE( data[0] == 0.5);
                REQUIRE( data[1] == 1.0);
                REQUIRE( data[2] == 1.5);
                REQUIRE( data[3] == 2.);
            }
            THEN( "check /= array" ) {
                data /= core::NumericalData<double, 4>(1, 2, 3, 2);
                REQUIRE( data[0] == 1.);
                REQUIRE( data[1] == 1.);
                REQUIRE( data[2] == 1.);
                REQUIRE( data[3] == 2.);
            }
            THEN( "check /= array dynamic" ) {
                data /= (core::NumericalData<double>(4) << -4, 2, 1, 5).finished();
                REQUIRE( data[0] == -0.25);
                REQUIRE( data[1] == 1.);
                REQUIRE( data[2] == 3.);
                REQUIRE( data[3] == 0.8);
            }
            THEN( "check array / scalar" ) {
                auto coeffSum = data/5.0;
                REQUIRE( coeffSum[0] == 0.2);
                REQUIRE( coeffSum[1] == 0.4);
                REQUIRE( coeffSum[2] == 0.6);
                REQUIRE( coeffSum[3] == 0.8);
            }
            THEN( "check scalar / array" ) {
                auto coeffSum = 5.0/data;
                REQUIRE( coeffSum[0] == 5.0);
                REQUIRE( coeffSum[1] == 2.5);
                REQUIRE( coeffSum[2] == 5.0/3.0);
                REQUIRE( coeffSum[3] == 1.25);
            }
            THEN( "check division of two arrays" ) {
                auto coeffSum = data / data;
                REQUIRE( coeffSum[0] == 1);
                REQUIRE( coeffSum[1] == 1.);
                REQUIRE( coeffSum[2] == 1.);
                REQUIRE( coeffSum[3] == 1.);
            }
            THEN( "check / of two arrays and scalar" ) {
                auto coeffSum = data/(data*2.0);
                REQUIRE( coeffSum[0] == 0.5);
                REQUIRE( coeffSum[1] == 0.5);
                REQUIRE( coeffSum[2] == 0.5);
                REQUIRE( coeffSum[3] == 0.5);
            }
            THEN( "check / of two arrays and scalar 2" ) {
                auto coeffSum = (data*2.0)/data;
                REQUIRE( coeffSum[0] == 2);
                REQUIRE( coeffSum[1] == 2.);
                REQUIRE( coeffSum[2] == 2.);
                REQUIRE( coeffSum[3] == 2.);
            }
        }

        THEN( "check map methods" ) {
            THEN( "reverse" ) {
                core::Array1Dd reverse = data.reverse();
                REQUIRE( reverse[0] == 4.0);
                REQUIRE( reverse[1] == 3.0);
                REQUIRE( reverse[2] == 2.0);
                REQUIRE( reverse[3] == 1.0);
            }  
            THEN( "reverse in place" ) {
                data.reverseInPlace();
                REQUIRE( data[0] == 4.0);
                REQUIRE( data[1] == 3.0);
                REQUIRE( data[2] == 2.0);
                REQUIRE( data[3] == 1.0);
            }  
            THEN( "square" ) {
                core::Array1Dd squared = data.square();
                REQUIRE( squared[0] == 1.0);
                REQUIRE( squared[1] == 4.0);
                REQUIRE( squared[2] == 9.0);
                REQUIRE( squared[3] == 16.0);
            }  
            THEN( "sqrt" ) {
                data << 100.0, 4.0, 9.0, 16.0;
                core::Array1Dd sqrted = data.sqrt();
                REQUIRE( sqrted[0] == 10.0);
                REQUIRE( sqrted[1] == 2.0);
                REQUIRE( sqrted[2] == 3.0);
                REQUIRE( sqrted[3] == 4.0);
            }  
            THEN( "pow" ) {
                data << 1,2,3,4;
                core::Array1Dd sqrted = data.pow(3);
                REQUIRE( sqrted[0] == 1.0);
                REQUIRE( sqrted[1] == 8.0);
                REQUIRE( sqrted[2] == 27.0);
                REQUIRE( sqrted[3] == 64.0);
            }  
        }  

        THEN( "check reduce methods" ) {
            REQUIRE( data.mean() == 2.5);
            REQUIRE( data.sum() == 10.0);
            REQUIRE( data.minCoeff() == 1.0);
            REQUIRE( data.maxCoeff() == 4.0);
        }  

        THEN( "check all operations" ) {
            REQUIRE( (data > 0).all() == 1);
            REQUIRE( (data < 5).all() == 1);
            REQUIRE( (data < 3).all() == 0);
            REQUIRE( (data == 1).all() == 0);

            data << 1,1,1,1;
            REQUIRE( (data > 0).all() == 1);
            REQUIRE( (data < 5).all() == 1);
            REQUIRE( (data < 3).all() == 1);
            REQUIRE( (data == 1).all() == 1);
        }          

        THEN( "check any operations" ) {
            REQUIRE( (data > 0).any() == 1);
            REQUIRE( (data < 5).any() == 1);
            REQUIRE( (data < 3).any() == 1);
            REQUIRE( (data == 1).any() == 1);
            REQUIRE( (data == 5).any() == 0);
        }    
    }

    SCENARIO( "Test assignment" ) {
        core::NumericalData<double> data = core::NumericalData<double>::Ones(10);
        REQUIRE( data.to_vector() == std::vector<double>(10, 1.0));
    }

    SCENARIO( "Test custom operations" ) { 
        THEN( "check custom ramp method" ) {
            const core::NumericalData<double, 4> data(1,2,3,4);
            const core::NumericalData<double, 4> ramped = data.ramp(3);
            REQUIRE( ramped[0] == 0);
            REQUIRE( ramped[1] == 0);
            REQUIRE( ramped[2] == 3);
            REQUIRE( ramped[3] == 4);
        }               
        THEN( "check custom ramp in place method" ) {
            core::NumericalData<double, 4> data(1,2,3,4);
            data.rampInPlace(3);
            REQUIRE( data[0] == 0);
            REQUIRE( data[1] == 0);
            REQUIRE( data[2] == 3);
            REQUIRE( data[3] == 4);
        }    
        THEN( "check LLS" ) {
            core::NumericalData<double, 3> data(3, 8, 15);
            const core::NumericalData<double, 3> lls = data.LLS();
            REQUIRE( lls[0] == std::log(std::log(3) + 1) );
            REQUIRE( lls[1] == std::log(std::log(4) + 1) );
            REQUIRE( lls[2] == std::log(std::log(5) + 1) );
        } 
        THEN( "check LLS in place" ) {
            core::NumericalData<double, 3> data(3, 8, 15);
            data.LLSInPlace();
            REQUIRE( data[0] == std::log(std::log(3) + 1) );
            REQUIRE( data[1] == std::log(std::log(4) + 1) );
            REQUIRE( data[2] == std::log(std::log(5) + 1) );
        } 
        THEN( "check LLS -> ILLS" ) {
            core::NumericalData<double, 3> data(3.454e3, 8.34e6, 15.12e-4);
            data.LLSInPlace().inverseLLSInPlace();
            REQUIRE( data[0] == Approx(3.454e3) );
            REQUIRE( data[1] == Approx(8.34e6) );
            REQUIRE( data[2] == Approx(15.12e-4) );
        } 
        THEN( "check ILLS -> LLS" ) {
            core::NumericalData<double, 3> data(3.42e-3, 8.34e-1, 15.12e-4);
            data.inverseLLSInPlace();
            data.LLSInPlace();
            REQUIRE( data[0] == Approx(3.42e-3) );
            REQUIRE( data[1] == Approx(8.34e-1) );
            REQUIRE( data[2] == Approx(15.12e-4) );
        } 
        THEN( "check LLS -> ILLS chaining" ) {
            core::NumericalData<double, 3> data(3.454e3, 8.34e6, 15.12e-4);
            data.LLSInPlace().inverseLLSInPlace();
            REQUIRE( data[0] == Approx(3.454e3) );
            REQUIRE( data[1] == Approx(8.34e6) );
            REQUIRE( data[2] == Approx(15.12e-4) );
        } 
        THEN( "check midpoint shift" ) {
            core::NumericalData<double, 7> data(1, 4, 6, 2, 4, 2, 5);
            THEN( "check order 0" ) {
                data.midpointInPlace(0);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 6.0 );
                REQUIRE( data[3] == 2.0 );
                REQUIRE( data[4] == 4.0 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 1" ) {
                data.midpointInPlace(1);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 3.5 );
                REQUIRE( data[2] == 3.0 );
                REQUIRE( data[3] == 5.0 );
                REQUIRE( data[4] == 2.0 );
                REQUIRE( data[5] == 4.5 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 2" ) {
                data.midpointInPlace(2);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 2.5 );
                REQUIRE( data[3] == 3.0 );
                REQUIRE( data[4] == 5.5 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 3" ) {
                data.midpointInPlace(3);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 6.0 );
                REQUIRE( data[3] == 3.0 );
                REQUIRE( data[4] == 4.0 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 4" ) {
                data.midpointInPlace(4);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 6.0 );
                REQUIRE( data[3] == 2.0 );
                REQUIRE( data[4] == 4.0 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
        } 
        THEN( "check midpoint shift - dynamic array" ) {
            core::NumericalData<double> data(7);
            data << 1, 4, 6, 2, 4, 2, 5;
            THEN( "check order 0" ) {
                data.midpointInPlace(0);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 6.0 );
                REQUIRE( data[3] == 2.0 );
                REQUIRE( data[4] == 4.0 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 1" ) {
                data.midpointInPlace(1);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 3.5 );
                REQUIRE( data[2] == 3.0 );
                REQUIRE( data[3] == 5.0 );
                REQUIRE( data[4] == 2.0 );
                REQUIRE( data[5] == 4.5 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 2" ) {
                data.midpointInPlace(2);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 2.5 );
                REQUIRE( data[3] == 3.0 );
                REQUIRE( data[4] == 5.5 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 3" ) {
                data.midpointInPlace(3);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 6.0 );
                REQUIRE( data[3] == 3.0 );
                REQUIRE( data[4] == 4.0 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
            THEN( "check order 4" ) {
                data.midpointInPlace(4);
                REQUIRE( data[0] == 1.0 );
                REQUIRE( data[1] == 4.0 );
                REQUIRE( data[2] == 6.0 );
                REQUIRE( data[3] == 2.0 );
                REQUIRE( data[4] == 4.0 );
                REQUIRE( data[5] == 2.0 );
                REQUIRE( data[6] == 5.0 );
            }
        } 
        THEN( "check SNIP simple" ) {
            const core::NumericalData<double, 4> data(1,2,3,4);
            const core::NumericalData<double, 4> snipped = data.snip(20);
            REQUIRE( snipped[0] == Approx(1.0) );
            REQUIRE( snipped[1] == Approx(1.8333089468266763) );
            REQUIRE( snipped[2] == Approx(2.874216968707311) );
            REQUIRE( snipped[3] == Approx(4.0) );
        }       
        THEN( "check SNIP complex" ) {
            const core::NumericalData<double, 16> 
                data(1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21,32.4,1.2,3.4,5.2,123.3,23.2,4.1);
            const core::NumericalData<double, 16> snipped = data.snip(0);
            const core::NumericalData<double, 16> 
                expected(1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21,32.4,1.2,3.4,5.2,123.3,23.2,4.1);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, snipped);
        }         
        THEN( "check SNIP complex" ) {
            const core::NumericalData<double, 16> 
                data(1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21, 32.4, 1.2, 3.4, 5.2, 123.3, 23.2, 4.1);
            const core::NumericalData<double, 16> snipped = data.snip(4);
            const core::NumericalData<double, 16> 
                expected(1.0, 
                9.599840039956527, 
                2.2325487695967374, 
                2.1, 
                1.6262270466592428, 
                2.1984699168502932, 
                1.668357848457926, 
                2.3, 
                2.440060339020095, 
                2.494169045564714, 
                1.2, 
                2.6995733396237362, 
                3.2217712295589997, 
                3.3438927139799794, 
                22.267498846621255, 
                4.1);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, snipped);
        }     
        THEN( "check SNIP complex 2" ) {
            const core::NumericalData<double, 16> 
                data(1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21, 32.4, 1.2, 3.4, 5.2, 123.3, 23.2, 4.1);
            const core::NumericalData<double, 16> snipped = data.snip(20);
            const core::NumericalData<double, 16>             
                expected(1.0, 
                9.599840039956527, 
                2.2325487695967374, 
                2.1, 
                1.6262270466592428, 
                1.097732439852992, 
                1.668357848457926, 
                2.3, 
                2.440060339020095, 
                2.494169045564714, 
                1.2, 
                2.6995733396237362, 
                3.2217712295589997, 
                3.3438927139799794, 
                22.267498846621255, 
                4.1);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, snipped);
        }         
        THEN( "check SNIP2" ) {
            const core::NumericalData<double, 16> 
                data(1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21, 32.4, 1.2, 3.4, 5.2, 123.3, 23.2, 4.1);
            std::vector<int> iterations(20);
            std::generate(iterations.begin(), iterations.end(), [n = 1] () mutable { return n++; });
            const core::NumericalData<double, 16> snipped = data.snip(iterations.begin(), iterations.end());
            const core::NumericalData<double, 16>             
                expected(1.0, 
                9.599840039956527, 
                2.2325487695967374, 
                2.1, 
                1.6262270466592428, 
                1.097732439852992, 
                1.668357848457926, 
                2.3, 
                2.440060339020095, 
                2.494169045564714, 
                1.2, 
                2.6995733396237362, 
                3.2217712295589997, 
                3.3438927139799794, 
                22.267498846621255, 
                4.1);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, snipped);
        }             
        THEN( "check SNIP2 - range" ) {
            const core::NumericalData<double, 16> 
                data(1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21, 32.4, 1.2, 3.4, 5.2, 123.3, 23.2, 4.1);
            auto rn = util::range<int,1,21,1>();
            const core::NumericalData<double, 16> snipped = data.snip(rn.begin(), rn.end());
            const core::NumericalData<double, 16>             
                expected(1.0, 
                9.599840039956527, 
                2.2325487695967374, 
                2.1, 
                1.6262270466592428, 
                1.097732439852992, 
                1.668357848457926, 
                2.3, 
                2.440060339020095, 
                2.494169045564714, 
                1.2, 
                2.6995733396237362, 
                3.2217712295589997, 
                3.3438927139799794, 
                22.267498846621255, 
                4.1);
            REQUIRE_NUMERICS_APPROX_THE_SAME(expected, snipped);
        }    
    }

    SCENARIO( "Test numerical gradient" ) {
        // create a string for splitting
        core::NumericalData<double, 6> data( 1. ,  2.0,  4.0,  7.0, 11.0, 16.0);
        const core::NumericalData<double, 6> exp0( 1. ,  2.0,  4.0,  7.0, 11.0, 16.0);
        const core::NumericalData<double, 6> exp1( 1. ,  1.5,  2.5,  3.5,  4.5,  5. );
        const core::NumericalData<double, 6> exp2( 0.5,  0.75, 1.0,  1.0,  0.75, 0.5 );
        const core::NumericalData<double, 6> exp3( 0.25, 0.25, 0.125,  -0.125,  -0.25, -0.25 );

        THEN( "check grad" ) {
            THEN("check order 0"){
                const core::NumericalData<double, 6> re0 = data.gradient(0); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp0, re0);
            }
            THEN("check order 1"){
                const core::NumericalData<double, 6> re1 = data.gradient(1); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp1, re1);
                const core::NumericalData<double, 6> re10 = data.gradient(1).gradient(0); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp1, re10);
                const core::NumericalData<double, 6> re01 = data.gradient(0).gradient(1); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp1, re01);
            }
            THEN("check order 2"){
                const core::NumericalData<double, 6> re2 = data.gradient(2); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp2, re2);
                const core::NumericalData<double, 6> re11 = data.gradient(1).gradient(1); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp2, re11);
                const core::NumericalData<double, 6> re20 = data.gradient(2).gradient(0); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp2, re20);
                const core::NumericalData<double, 6> re02 = data.gradient(0).gradient(2); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp2, re02);
            }
            THEN("check order 3"){
                const core::NumericalData<double, 6> re3 = data.gradient(3); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, re3);
                const core::NumericalData<double, 6> re12 = data.gradient(1).gradient(2); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, re12);
                const core::NumericalData<double, 6> re21 = data.gradient(2).gradient(1); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, re21);
                const core::NumericalData<double, 6> re30 = data.gradient(3).gradient(0); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, re30);
                const core::NumericalData<double, 6> re111 = data.gradient(1).gradient(1).gradient(1); 
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, re111);
            }
        }
        THEN( "check grad in place" ) {
            THEN("check order 0"){
                data.gradientInPlace(0);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp0, data);
            }
            THEN("check order 1"){
                data.gradientInPlace(1);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp1, data);
                data.gradientInPlace(0);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp1, data);
            }
            THEN("check order 2"){
                data.gradientInPlace(2);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp2, data);
                data.gradientInPlace(0);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp2, data);
            }
            THEN("check order 2 - v2"){
                data.gradientInPlace(1);
                data.gradientInPlace(1);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp2, data);
            }
            THEN("check order 3"){
                data.gradientInPlace(3);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, data);
                data.gradientInPlace(0);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, data);
            }
            THEN("check order 3 - v2"){
                data.gradientInPlace(0);
                data.gradientInPlace(1);
                data.gradientInPlace(2);
                REQUIRE_NUMERICS_APPROX_THE_SAME(exp3, data);
            }
        }
    }

    SCENARIO( "Test numerical array iterator" ) {
        // create a string for splitting
        core::NumericalData<int, 20> data(1,2,3,24,5,36,7,8,9,10,11,12,133,-14,15,216,17,18,19,20);
        core::NumericalData<int, 20> retrieved = core::NumericalData<int, 20>::Zero();

        THEN( "check slice" ) {
            const int stepsize = 4;
            auto chunkindices = util::range<int, 0, 20, stepsize>(); // 0, 4, 8, 12, 16, 20
            for(auto it=chunkindices.begin(); it!=std::prev(chunkindices.end()); ++it){
                //take a slice between chunk indices
                auto newdata = data(*it, *std::next(it, stepsize));
                for(int i=0;i<newdata.size();++i){
                    retrieved[i+*it] = newdata[i];
                }
            }
            REQUIRE_NUMERICS_THE_SAME<int>(data, retrieved);
        }
        THEN( "check slice 2 - step size does not cover full range" ) {
            const int stepsize = 3;
            auto chunkindices = util::range<int, 0, 20, stepsize>(); // 0, 3, 6, 9, 12, 15, 18
            for(auto it=chunkindices.begin(); it!=std::prev(chunkindices.end()); ++it){
                //take a slice between chunk indices
                auto newdata = data(*it, std::min(*std::next(it, stepsize), *chunkindices.end()));
                for(int i=0;i<newdata.size();++i){
                    retrieved[i+*it] = newdata[i];
                }
            }
            REQUIRE_NUMERICS_THE_SAME<int>(data, retrieved);
        }
    }

    SCENARIO( "numerical data window - int" ) {
        constexpr int value_size = 9;
        core::NumericalData<int, value_size> values = {45, 2, 78, 23, 23, 89, 27, 23, 67};
        
        int nouter = 1;
        int ninner = 0;
        THEN( "check nouter=1, ninner=0" ) {
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,2>(45, 78), core::window<int,value_size,2>(values, 1, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,3>(45, 2, 78), core::window<int,value_size,3>(values, 1, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,1>(2), core::window<int,value_size,1>(values, 0, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,2>(45, 2), core::window<int,value_size,2>(values, 0, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,1>(23), core::window<int,value_size,1>(values, 8, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,2>(23, 67), core::window<int,value_size,2>(values, 8, nouter, ninner, true));
        }      
        
        nouter = 0;
        ninner = 0;
        THEN( "check nouter=0, ninner=0" ) {
            const core::NumericalData<int,0> empty;
            REQUIRE_NUMERICS_THE_SAME<int>(empty, core::window<int,value_size,0>(values, 1, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,1>(2), core::window<int,value_size,1>(values, 1, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>(empty, core::window<int,value_size,0>(values, 0, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,1>(45), core::window<int,value_size,1>(values, 0, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>(empty, core::window<int,value_size,0>(values, 8, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>(core::NumericalData<int,1>(67), core::window<int,value_size,1>(values, 8, nouter, ninner, true));
        }    
        
        nouter = 4;
        ninner = 0;
        THEN( "check nouter=4, ninner=0" ) {
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,5>(45, 78, 23, 23, 89), core::window<int,value_size,5>(values, 1, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,6>(45, 2, 78, 23, 23, 89), core::window<int,value_size,6>(values, 1, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,4>(2, 78, 23, 23), core::window<int,value_size,4>(values, 0, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,5>(45, 2, 78, 23, 23), core::window<int,value_size,5>(values, 0, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,4>(23, 89, 27, 23), core::window<int,value_size,4>(values, 8, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,5>(23, 89, 27, 23, 67), core::window<int,value_size,5>(values, 8, nouter, ninner, true));
        }      
        
        nouter = 4;
        ninner = 2;
        THEN( "check nouter=4, ninner=2" ) {
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,4>(45, 2, 23, 67), core::window<int,value_size,4>(values, 4, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,5>(45, 2, 23, 23, 67), core::window<int,value_size,5>(values, 4, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,2>(23, 23), core::window<int,value_size,2>(values, 0, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,3>(45, 23, 23), core::window<int,value_size,3>(values, 0, nouter, ninner, true));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,2>(23, 89), core::window<int,value_size,2>(values, 8, nouter, ninner, false));
            REQUIRE_NUMERICS_THE_SAME<int>( core::NumericalData<int,3>(23, 89, 67), core::window<int,value_size,3>(values, 8, nouter, ninner, true));
        }          
    }

PEAKINGDUCK_NAMESPACE_END // unittests
PEAKINGDUCK_NAMESPACE_END // peakingduck