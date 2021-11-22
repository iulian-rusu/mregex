cmake_minimum_required(VERSION 3.16)
project(mregex VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++20 -O3 -Wall -Wextra -Wpedantic -Wuninitialized -Wshadow)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fconcepts-diagnostics-depth=3)
endif()

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
        test/iterator_api_tests.cpp
)