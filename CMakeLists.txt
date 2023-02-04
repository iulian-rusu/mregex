cmake_minimum_required(VERSION 3.16)
project(mregex VERSION 2.0 LANGUAGES CXX)

set(PROJECT_DESCRIPTION "Metaprogramming-based, compile-time implementation of regular expressions in C++20.")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler options
if(MSVC)
    add_compile_options(/W4)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(/Od)
    else()
        add_compile_options(/O2)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wuninitialized -Wshadow -Wunused)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-Og)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Main library target
add_library(mregex INTERFACE)
target_include_directories(mregex INTERFACE include/)

# Building tests
option(MREGEX_BUILD_TESTS "build tests" ON)
if(${MREGEX_BUILD_TESTS})
    add_subdirectory(test/)
endif()

# Building examples
option(MREGEX_BUILD_EXAMPLES "build examples" ON)
if(${MREGEX_BUILD_EXAMPLES})
    add_subdirectory(example/)
endif()