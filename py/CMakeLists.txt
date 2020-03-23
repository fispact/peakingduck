# we use uppercase to dictate the raw c++ bindings
# lowercase is the python wrapper that uses the bindings
set(PEAKINGDUCK_PY_NAME PEAKINGDUCK)
set(PY_SOURCES peakingduck.cpp)

# OS specific stuff
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

# Build and link the pylib module
pybind11_add_module(${PEAKINGDUCK_PY_NAME} MODULE ${PY_SOURCES})
target_link_libraries(${PEAKINGDUCK_PY_NAME}
  PRIVATE
    ${HEADER_LIB_NAME}
)

set_target_properties(${PEAKINGDUCK_PY_NAME} PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR}
  ARCHIVE_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${PY_OUTPUT_DIR}
)

if(MINGW)
  message(STATUS "Using static linking with MinGW")
  message(STATUS "Disabling LTO with static linking")
  target_compile_options(${PEAKINGDUCK_PY_NAME}
    PRIVATE
      -fno-lto
  )
  target_link_libraries(${PEAKINGDUCK_PY_NAME}
    PRIVATE
      -fno-lto
      -static
      -static-libstdc++
      -static-libgcc
  )
endif()
