set(CPP_UNIT_TESTS_NAME peakingduckcpptests)

set(CPP_UNIT_TESTS_SOURCES
  main.cpp
  test_util.cpp
  test_numerical.cpp
  test_process.cpp
  test_smoothing.cpp
)

add_executable(${CPP_UNIT_TESTS_NAME} ${CPP_UNIT_TESTS_SOURCES})

target_link_libraries(${CPP_UNIT_TESTS_NAME}
  PRIVATE
    ${HEADER_LIB_NAME}
    Catch2::Catch2
)

set_target_properties(${CPP_UNIT_TESTS_NAME} PROPERTIES
  VERSION ${PEAKINGDUCK_VERSION}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
