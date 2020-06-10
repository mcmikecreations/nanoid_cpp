cmake_minimum_required(VERSION 3.5)
project (nanoid_cpp
    VERSION 2.1.0
    )
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(vendor/catch2)

file(GLOB_RECURSE NANOID_SRC_FILES src/*.cpp)
file(GLOB_RECURSE NANOID_INC_FILES inc/*.h)

add_library(nanoid ${NANOID_SRC_FILES})
target_include_directories(nanoid PUBLIC inc)
#target_compile_options(nanoid PRIVATE
#  $<$<CXX_COMPILER_ID:MSVC>:/W4>
#  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
#)

#############################################
# Unit tests

enable_testing()
add_executable(nanoid_tests tests/unit_tests.cpp)

target_link_libraries(nanoid_tests
    nanoid
    Catch2::Test
)

add_test(test_all nanoid_tests)