# Find default python libraries and interpreter
set(Python_ADDITIONAL_VERSIONS 3.6)
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)

set(PEAKINGDUCK_PY_NAME pykingduck)

message(STATUS "Using PyBind")
set(PYBIND_INC_DIR ${PYBIND_INCLUDES})
include_directories(${PROJECT_DIR_BASE} ${PYBIND_INC_DIR} ${PYTHON_INCLUDE_DIRS} ${UNITS_INCLUDES})

set(PY_SOURCES peakingduck.cpp)

# OS specific stuff
if(APPLE)
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

# Build and link the pylib module
add_library(${PEAKINGDUCK_PY_NAME} SHARED ${PY_SOURCES})

target_link_libraries(${PEAKINGDUCK_PY_NAME} ${PYTHON_LIBRARIES})

# Tweaks the name of the library to match what Python expects
set_target_properties(${PEAKINGDUCK_PY_NAME} PROPERTIES SUFFIX .so)
set_target_properties(${PEAKINGDUCK_PY_NAME} PROPERTIES PREFIX "")

# Set the exe properties
set_target_properties(${PEAKINGDUCK_PY_NAME}
                      PROPERTIES OUTPUT_NAME ${PEAKINGDUCK_PY_NAME}
                      VERSION ${PEAKINGDUCK_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR}
                      ARCHIVE_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR})