cmake_minimum_required(VERSION 3.15)
project(gsl_cpp_wrapper LANGUAGES CXX)

if (NOT DEFINED gsl_cpp_wrapper_MASTER_PROJECT)
  set(gsl_cpp_wrapper_MASTER_PROJECT OFF)
  if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(gsl_cpp_wrapper_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
  endif ()
endif ()

if(gsl_cpp_wrapper_MASTER_PROJECT)
  # prevent for in-source building
  set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
  set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
  if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "In-source builds are not allowed.")
  endif ()

  enable_testing()

  set(CMAKE_BUILD_TYPE Debug) #Debug
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
  set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
  
  include_directories("include")
  add_subdirectory("test")

else(gsl_cpp_wrapper_MASTER_PROJECT)
add_subdirectory("src")
endif(gsl_cpp_wrapper_MASTER_PROJECT)







