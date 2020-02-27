cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# cannot have a cmake project name the same as a target or
# library name (use pp for ++ to indicate C++)
PROJECT(peakingduckpp LANGUAGES CXX)

# cmake options
OPTION(BUILD_TESTS "Build unit tests" ON)
OPTION(BUILD_PY_BINDINGS "Build python bindings" ON)
OPTION(BUILD_DOCS "Build the documentation" OFF)

set (PROJECT_DIR_BASE ${CMAKE_CURRENT_LIST_DIR}/include)
set (PYTHON_PROJECT_DIR_BASE ${CMAKE_CURRENT_LIST_DIR}/py)
set (TESTS_DIR_BASE ${CMAKE_CURRENT_LIST_DIR}/tests)
set (THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty)
set (PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR})

find_package( PythonInterp 3.6 REQUIRED )
find_package( PythonLibs 3.6 REQUIRED )
#find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# Eigen3 is header only
set (EIGEN_INCLUDES ${THIRD_PARTY_DIR}/eigen)

# catch2 is header only
set (CATCH2_INCLUDES ${THIRD_PARTY_DIR}/Catch2/single_include/catch2)

# pybind11 is header only
set (PYBIND_INCLUDES ${THIRD_PARTY_DIR}/pybind11/include)

# units is header only
set (UNITS_INCLUDES ${THIRD_PARTY_DIR}/units/include)

# set the output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(PY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/py)

# Get the version, set in .VERSION file
file ( STRINGS "${CMAKE_SOURCE_DIR}/.VERSION" VERSION )
string( REPLACE "." ";" VERSION_LIST ${VERSION} )
list(GET VERSION_LIST 0 VERSION_MAJOR)
list(GET VERSION_LIST 1 VERSION_MINOR)
list(GET VERSION_LIST 2 VERSION_PATCH)
set(PEAKINGDUCK_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message ( STATUS "CMake build configuration for PEAKING DUCK ${PEAKINGDUCK_VERSION}" )

# C++ flags
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

set(PEAKINGDUCK_CPP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/common.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/constants.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/exceptions.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/io.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/peakingduck.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/io/spectralio.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/process.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/numericalfunctions.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/smoothing.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/numerical.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/peaking.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/spectral.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/crtp.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/range.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/string.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/file.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/window.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/stream.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util.hpp
  )

# header-only library target. To use this project as a subdirectory,
# add the following to your code:
#
# add_subdirectory(peakingduck) # or whatever you named the directory
# target_link_libraries(${PROJECT_NAME} peakingduck)
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} 
	INTERFACE $<BUILD_INTERFACE:${PROJECT_DIR_BASE}> 
	$<INSTALL_INTERFACE:include> 
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# enable parallel build
	set( ENV{CL} /MP )
	target_compile_definitions(${PROJECT_NAME} INTERFACE "/permissive-")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# Increase warning levels
	add_compile_options(-Wall -Wextra -pedantic)
endif()

# python bindings
if(BUILD_PY_BINDINGS)
        add_subdirectory(${THIRD_PARTY_DIR}/pybind11)
	add_subdirectory(${PYTHON_PROJECT_DIR_BASE})
endif(BUILD_PY_BINDINGS)

# unit tests
if(BUILD_TESTS)
	add_subdirectory(${TESTS_DIR_BASE}/cpp)
endif(BUILD_TESTS)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/peakingduckConfigVersion.cmake"
  VERSION ${PEAKINGDUCK_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(TARGETS peakingduckpp
    EXPORT peakingduckConfig
)

install(EXPORT peakingduckConfig
    NAMESPACE peakingduck::
    DESTINATION share/peakingduck/cmake
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/peakingduckConfigVersion.cmake"
    DESTINATION share/units/cmake
)

install(DIRECTORY include/
    DESTINATION include
)
