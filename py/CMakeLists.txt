# Find default python libraries and interpreter
set(Python_ADDITIONAL_VERSIONS 3.6)
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)

# we use uppercase to dictate the raw c++ bindings
# lowercase is the python wrapper that uses the bindings
set(PEAKINGDUCK_PY_NAME PEAKINGDUCK)

message(STATUS "Using PyBind")
set(PYBIND_INC_DIR ${PYBIND_INCLUDES})
include_directories(${PROJECT_DIR_BASE} ${PYBIND_INC_DIR} ${PYTHON_INCLUDE_DIRS} ${UNITS_INCLUDES} ${EIGEN_INCLUDES})

# ======================= Generate docstrings =================================================
set(MKDOCSTRINGS ${CMAKE_SOURCE_DIR}/thirdparty/pybind11/tools/mkdoc.py)

function(generate_docstrings_from_header MKDOC_EXECUTABLE HEADER_FILE DOCSTRING_FILE COMPILER_FLAGS)
  add_custom_command(OUTPUT ${DOCSTRING_FILE}
                     COMMAND ${MKDOC_EXECUTABLE} ${COMPILER_FLAGS} -o ${DOCSTRING_FILE} ${HEADER_FILE}
		     DEPENDS ${HEADER_FILE}
		     )
endfunction()

function(add_docstrings_to_target TARGET DOCSTRING_INCLUDE_FILE)
  get_filename_component(docstring_header_name ${DOCSTRING_INCLUDE_FILE} NAME_WE)
  string(TOUPPER ${docstring_header_name} docstring_macro)
  set(docstring_macro "${docstring_macro}_HPP")

  get_filename_component(DOCSTRING_INCLUDE_FILE_abs ${DOCSTRING_INCLUDE_FILE} ABSOLUTE)
  get_filename_component(docstring_include_dir ${DOCSTRING_INCLUDE_FILE_abs} DIRECTORY)
  get_filename_component(docstring_include_filename ${DOCSTRING_INCLUDE_FILE} NAME)
  file(RELATIVE_PATH reldir ${CMAKE_CURRENT_SOURCE_DIR} ${docstring_include_dir})
  get_filename_component(docstring_output_dir "${CMAKE_CURRENT_BINARY_DIR}/${reldir}" ABSOLUTE)
  set(DOCSTRING_INCLUDE_OUTPUT "${docstring_output_dir}/${docstring_include_filename}")

  set(allheaders ${DOCSTRING_INCLUDE_OUTPUT})
  set(docstring_inc_file_contents "#ifndef ${docstring_macro} \n#define ${docstring_macro} \n")

  get_target_property(include_dirs ${TARGET} INCLUDE_DIRECTORIES)
  get_target_property(standard ${TARGET} CXX_STANDARD)
  if (NOT standard STREQUAL "NOTFOUND")
    set(COMPILE_FLAGS "-std=c++${standard}")
  else()
    set(COMPILE_FLAGS "")
  endif()    
  foreach(dir ${include_dirs})
    list(APPEND COMPILE_FLAGS "-I${dir}")
  endforeach()
  
  foreach(header ${ARGN})
    get_filename_component(header_abs ${header} ABSOLUTE)
    get_filename_component(header_dir ${header_abs} DIRECTORY)
    file(RELATIVE_PATH reldir ${CMAKE_CURRENT_SOURCE_DIR} "${header_dir}")
    get_filename_component(docheader_dir "${CMAKE_CURRENT_BINARY_DIR}/${reldir}" ABSOLUTE)
    get_filename_component(header_filename ${header} NAME_WE)
    set(docheader "${docheader_dir}/${header_filename}_docstrings.h")
    file(MAKE_DIRECTORY "${docheader_dir}")

    list(APPEND allheaders ${docheader})
    generate_docstrings_from_header(${MKDOCSTRINGS} ${header} ${docheader} "${COMPILE_FLAGS}")
    set(docstring_inc_file_contents "${docstring_inc_file_contents}#include \"${docheader}\"\n")
  endforeach()
  set(docstring_inc_file_contents "${docstring_inc_file_contents}#endif\n")

  # Check if main include file already has been generated. If so, only write new
  # file if contents has changed. This avoid recompiling after reconfiguring cmake.
  set (SHOULD_WRITE_INC_FILE True)
  if (EXISTS ${DOCSTRING_INCLUDE_OUTPUT})
    file (READ ${DOCSTRING_INCLUDE_OUTPUT} existing_file)
    if (${existing_file} STREQUAL ${docstring_inc_file_contents})
      set (SHOULD_WRITE_INC_FILE False)
    endif()
  endif()
  if (${SHOULD_WRITE_INC_FILE})
    file (WRITE ${DOCSTRING_INCLUDE_OUTPUT} ${docstring_inc_file_contents})
  endif()

  target_sources(${TARGET} PRIVATE ${allheaders})
  target_include_directories(${TARGET} PRIVATE ${docstring_output_dir})
endfunction()
# ======================= End generate docstrings =============================================

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

add_docstrings_to_target(${PEAKINGDUCK_PY_NAME} "docstrings.hpp" ${PEAKINGDUCK_CPP_SOURCES})
