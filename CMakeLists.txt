cmake_minimum_required(VERSION 3.16)
project(mregex)

include_directories(include)
add_executable(
        demo
        demo.cpp
        test/stack_tests.cpp
        test/traits_tests.cpp
        test/parser_ast_tests.cpp
        test/parser_accept_tests.cpp
        test/parser_error_tests.cpp
        test/flag_tests.cpp
        test/regex_match_tests.cpp
        test/regex_search_tests.cpp
        test/regex_union_tests.cpp
        test/regex_generator_tests.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -O3")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts-diagnostics-depth=3")
endif()