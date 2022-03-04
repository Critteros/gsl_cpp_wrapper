cmake_minimum_required(VERSION 3.15)

find_package(GSL REQUIRED)
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library("gslwrapper" ${SOURCES})

target_link_options("gslwrapper" PRIVATE -fsanitize=address -fsanitize=leak)
target_link_libraries("gslwrapper" GSL::gsl GSL::gslcblas)
target_compile_options("gslwrapper" PRIVATE -Wextra -Wpedantic)