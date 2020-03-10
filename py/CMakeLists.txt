# Find default python libraries and interpreter
#set(Python_ADDITIONAL_VERSIONS 3.6)
#find_package(PythonInterp 3 REQUIRED)
#find_package(PythonLibs 3 REQUIRED)

# we use uppercase to dictate the raw c++ bindings
# lowercase is the python wrapper that uses the bindings
set(PEAKINGDUCK_PY_NAME PEAKINGDUCK)

message(STATUS "Using PyBind")
#set(PYBIND_INC_DIR ${PYBIND_INCLUDES})
#include_directories(${PROJECT_DIR_BASE} ${PYBIND_INC_DIR} ${PYTHON_INCLUDE_DIRS} ${UNITS_INCLUDES} ${EIGEN_INCLUDES})

set(PY_SOURCES peakingduck.cpp)

# OS specific stuff
if(APPLE)
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

# Build and link the pylib module
pybind11_add_module(${PEAKINGDUCK_PY_NAME} MODULE ${PY_SOURCES})
target_include_directories(${PEAKINGDUCK_PY_NAME} PRIVATE
  ${UNITS_INCLUDES} ${EIGEN_INCLUDES} ${PROJECT_DIR_BASE})
# Tweaks the name of the library to match what Python expects
#set_target_properties(${PEAKINGDUCK_PY_NAME} PROPERTIES SUFFIX .so)
#set_target_properties(${PEAKINGDUCK_PY_NAME} PROPERTIES PREFIX "")

# Set the exe properties
set_target_properties(${PEAKINGDUCK_PY_NAME}
                      PROPERTIES #OUTPUT_NAME ${PEAKINGDUCK_PY_NAME}
#                      VERSION ${PEAKINGDUCK_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR}
                      ARCHIVE_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR})

if(MINGW)
  message(STATUS "Using static linking with MinGW")
  message(STATUS "Disabling LTO with static linking")
  target_compile_options(${PEAKINGDUCK_PY_NAME} PRIVATE -fno-lto)
  target_link_libraries(${PEAKINGDUCK_PY_NAME} PRIVATE -fno-lto
    -static -static-libstdc++ -static-libgcc)
endif()
