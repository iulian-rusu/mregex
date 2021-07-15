cmake_minimum_required(VERSION 3.19)
project(cx)

set(CMAKE_CXX_STANDARD 20)

add_executable(cx main.cpp cx/grammar.h cx/utility/static_string.h cx/symbol.h cx/parser.h cx/utility/stack.h cxregex.h tests/parser_accept_tests.cpp tests/stack_tests.cpp tests/tests.h tests/utility_tests.cpp cx/ast/ast.h tests/parser_ast_tests.cpp cx/regex.h cx/regex_result.h tests/regex_match_tests.cpp cx/ast/char_class.h cx/ast/capture_indexer.h cx/ast/ast_builder.h cx/utility/meta_helpers.h cx/ast/capture_counter.h cx/utility/concepts.h cx/ast/backref_builder.h cx/match_result.h cx/generator.h cx/ast/atomic.h cx/match_context.h cx/ast/capture.h cx/utility/tuple_helpers.h cx/regex_flags.h cx/ast/ast_decl.h cx/utility/char_helpers.h cx/ast/match_params.h)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fconcepts -O3")