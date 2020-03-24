cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(peakingduck_cpp_examples LANGUAGES CXX)

find_package(peakingduck)
# In general you do not need pybind to build a C++ project which uses
# peakingduck. However, in this particular example we wanto to access
# a SciPy function using pybind.
add_subdirectory(pybind11)

add_executable(processes processes.cpp)
target_link_libraries(processes
  PRIVATE
    peakingduck::peakingduck
    pybind11::embed
)

