cmake_minimum_required(VERSION 3.5)
project (nanoid_cpp
    VERSION 2.1.0
    )
set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

option(BUILD_TESTS "Build nanoid unit tests" ON)
option(BUILD_EXAMPLES "Build nanoid usage examples" ON)

add_subdirectory(vendor/catch2)

file(GLOB_RECURSE NANOID_SRC_FILES src/*.cpp)
file(GLOB_RECURSE NANOID_INC_FILES inc/*.h)

add_library(nanoid ${NANOID_SRC_FILES})
target_include_directories(nanoid PUBLIC inc)
target_link_libraries(nanoid Threads::Threads)
#target_compile_options(nanoid PRIVATE
#  $<$<CXX_COMPILER_ID:MSVC>:/W4>
#  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
#)

#############################################
# Examples
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
    message(STATUS "Added examples to build")
endif()

#############################################
# Unit tests
if (BUILD_TESTS)
    enable_testing()
    add_executable(nanoid_tests tests/unit_tests.cpp)

    target_link_libraries(nanoid_tests
            nanoid
            Catch2::Test
            )

    add_test(test_all nanoid_tests)
    message(STATUS "Added tests to build")
endif()