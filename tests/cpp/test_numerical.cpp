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
    }

PEAKINGDUCK_NAMESPACE_END // unittests
PEAKINGDUCK_NAMESPACE_END // peakingduck