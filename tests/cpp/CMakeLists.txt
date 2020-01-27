project (CPPUNITTESTS)

set (CPP_UNIT_TESTS_NAME peakingduckcpptests)

set(CPP_UNIT_TESTS_SOURCES
    main.cpp
    test_util.cpp
    test_numerical.cpp
    test_smoothing.cpp)

# Include directories
include_directories (${CATCH2_INCLUDES}
                     ${UNITS_INCLUDES}
                     ${EIGEN_INCLUDES}
                     ${PROJECT_DIR_BASE})

add_executable (${CPP_UNIT_TESTS_NAME} ${CPP_UNIT_TESTS_SOURCES})

set_target_properties(${CPP_UNIT_TESTS_NAME}
                      PROPERTIES OUTPUT_NAME ${CPP_UNIT_TESTS_NAME}
                      VERSION ${PEAKINGDUCK_VERSION}
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR}/bin)
