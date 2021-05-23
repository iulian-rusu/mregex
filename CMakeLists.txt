cmake_minimum_required(VERSION 3.19)
project(cx)

set(CMAKE_CXX_STANDARD 20)

add_executable(cx main.cpp cx/grammar.h cx/static_string.h cx/symbol.h cx/parser.h cx/stack.h cxregex.h tests/parser_accept_tests.cpp tests/stack_tests.cpp tests/tests.h cx/utility.h tests/utility_tests.cpp cx/ast.h tests/parser_ast_tests.cpp cx/regex.h cx/regex_result.h tests/regex_match_tests.cpp cx/char_class.h cx/capture_builder.h cx/ast_builder.h)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fconcepts -O3")