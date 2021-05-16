cmake_minimum_required(VERSION 3.19)
project(ctr)

set(CMAKE_CXX_STANDARD 20)

add_executable(ctr main.cpp ctr/grammar.h ctr/static_string.h ctr/symbol.h ctr/parser.h ctr/stack.h ctr.h tests/parser_accept_tests.cpp tests/stack_tests.cpp tests/tests.h ctr/utility.h tests/utility_tests.cpp ctr/ast.h tests/parser_ast_tests.cpp ctr/matching.h ctr/result.h)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fconcepts -O3")