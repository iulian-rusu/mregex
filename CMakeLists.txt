cmake_minimum_required(VERSION 3.16)
project(mregex VERSION 2.0 LANGUAGES CXX)

set(PROJECT_DESCRIPTION "Metaprogramming-based, compile-time implementation of regular expressions in C++20.")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(/Od)
    else()
        add_compile_options(/O2)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wuninitialized -Wshadow -Wunused)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-Og)
    else()
        add_compile_options(-O3)
    endif()
endif()

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