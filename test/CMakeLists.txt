cmake_minimum_required(VERSION 3.15)

find_package(GSL REQUIRED)
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

set(TARGET_NAME "tests")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  ${TARGET_NAME}
  ${SOURCES}
)
target_link_libraries(
  ${TARGET_NAME}
  gtest_main
  gslwrapper
)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})


target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address -fsanitize=leak)
target_compile_options(${TARGET_NAME} PRIVATE -Wextra -Wpedantic)