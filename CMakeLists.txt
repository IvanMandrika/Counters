cmake_minimum_required(VERSION 3.10)
project(Counters)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_library(counter_checker_lib
        src/counters.cpp
        src/counters.h
)

add_executable(main
        src/main.cpp
)
target_link_libraries(main counter_lib)

add_executable(tests
        tests/tests.cpp
)
target_link_libraries(tests
        PRIVATE
        counter_lib
        GTest::GTest
        GTest::Main
)

enable_testing()
add_test(NAME Counters COMMAND tests)
