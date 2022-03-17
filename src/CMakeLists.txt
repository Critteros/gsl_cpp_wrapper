cmake_minimum_required(VERSION 3.15)

find_package(GSL REQUIRED)

add_library(gsl_cpp_wrapper INTERFACE)
target_include_directories(gsl_cpp_wrapper INTERFACE "../include")
target_link_libraries(gsl_cpp_wrapper INTERFACE GSL::gsl GSL::gslcblas)