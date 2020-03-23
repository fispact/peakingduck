cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# cannot have a cmake project name the same as a target or
# library name (use pp for ++ to indicate C++)
project(peakingduck LANGUAGES CXX)
set(HEADER_LIB_NAME ${PROJECT_NAME})

include(GNUInstallDirs)

# cmake options
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_PY_BINDINGS "Build python bindings" ON)
#option(BUILD_DOCS "Build the documentation" OFF)

option(USE_SYSTEM_PYBIND "Use a pre-installed version of PyBind" OFF)
option(USE_SYSTEM_EIGEN "Use a pre-installed version of Eigen" OFF)
option(USE_SYSTEM_UNITS "Use a pre-installed version of Units" OFF)
option(USE_SYSTEM_CATCH2 "Use a pre-installed version of Catch2" OFF)


# set various source directories in this project
set(PROJECT_DIR_BASE ${CMAKE_CURRENT_LIST_DIR}/include)
set(PYTHON_PROJECT_DIR_BASE ${CMAKE_CURRENT_LIST_DIR}/py)
set(TESTS_DIR_BASE ${CMAKE_CURRENT_LIST_DIR}/tests)
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty)


# load dependencies
if(USE_SYSTEM_EIGEN)
  find_package(Eigen3 REQUIRED NO_MODULE)
else()
  set(BUILD_TESTING OFF CACHE STRING "Enable creation of Eigen tests.")
  add_subdirectory(${THIRD_PARTY_DIR}/eigen)
endif()

if(USE_SYSTEM_UNITS)
  find_package(units REQUIRED)
else()
  set(BUILD_TESTS_TMP ${BUILD_TESTS})
  set(BUILD_TESTS OFF CACHE STRING
      "Temporarily turn of building unit tests while importing units" FORCE)
  add_subdirectory(${THIRD_PARTY_DIR}/units)
  set(BUILD_TESTS ${BUILD_TESTS_TMP} CACHE STRING "Build unit tests" FORCE)
  add_library(units::units ALIAS units)
endif()

if(BUILD_TESTS)
  if(USE_SYSTEM_CATCH2)
    find_package(Catch2 REQUIRED)
  else()
    list(APPEND CMAKE_MODULE_PATH ${THIRD_PARTY_DIR}/catch2/cmake)
    add_subdirectory(${THIRD_PARTY_DIR}/Catch2)
  endif()
endif()

if(BUILD_PY_BINDINGS)
  if(USE_SYSTEM_PYBIND)
    find_package(pybind11 REQUIRED)
  else()
    add_subdirectory(${THIRD_PARTY_DIR}/pybind11)
  endif()
endif(BUILD_PY_BINDINGS)


# set the output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(PY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/py)


# Get the version, set in .VERSION file
file(STRINGS "${CMAKE_SOURCE_DIR}/.VERSION" VERSION)
string(REPLACE "." ";" VERSION_LIST ${VERSION})
list(GET VERSION_LIST 0 VERSION_MAJOR)
list(GET VERSION_LIST 1 VERSION_MINOR)
list(GET VERSION_LIST 2 VERSION_PATCH)
set(PEAKINGDUCK_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message(STATUS "CMake build configuration for PEAKING DUCK ${PEAKINGDUCK_VERSION}")


# C++ flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra" CACHE STRING "Compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG -g -O0 --coverage -fprofile-arcs
  -ftest-coverage -fno-inline -fno-inline-small-functions
  -fno-default-inline)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

# header-only library target. To use this project as a subdirectory,
# add the following to your code:
#
# add_subdirectory(peakingduck) # or whatever you named the directory
# target_link_libraries(${PROJECT_NAME} peakingduck)

add_library(${HEADER_LIB_NAME} INTERFACE)
set(HEADERLIB_DEPENDENCIES "Eigen3::Eigen units::units")
target_link_libraries(${HEADER_LIB_NAME}
  INTERFACE
    Eigen3::Eigen
    units::units
)
target_include_directories(${HEADER_LIB_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}> 
)

# Why is this here? The compile flags set above don't support VS compilers...
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
  add_subdirectory(${PYTHON_PROJECT_DIR_BASE})
endif(BUILD_PY_BINDINGS)

# unit tests
if(BUILD_TESTS)
  add_subdirectory(${TESTS_DIR_BASE}/cpp)
endif(BUILD_TESTS)

# documentation
if(BUILD_DOCS)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
      COMMENT "Generating API documentation with Doxygen" VERBATIM
      )
  endif(DOXYGEN_FOUND)
endif()


# install the header-only library
include(CMakePackageConfigHelpers)
set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PEAKINGDUCK_VERSION}
  COMPATIBILITY SameMajorVersion
)
configure_package_config_file(${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
  PATH_VARS CONFIG_INSTALL_DIR
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION ${CONFIG_INSTALL_DIR}
)

install(TARGETS ${HEADER_LIB_NAME}
  EXPORT ${PROJECT_NAME}
)
install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CONFIG_INSTALL_DIR}
)
