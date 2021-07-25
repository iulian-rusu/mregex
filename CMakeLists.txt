cmake_minimum_required(VERSION 3.19)
project(cx)

set(CMAKE_CXX_STANDARD 20)

add_executable(cx main.cpp cx/grammar.h cx/utility/static_string.h cx/symbol.h cx/parser.h cx/utility/stack.h cxregex.h tests/parser_accept_tests.cpp tests/stack_tests.cpp tests/tests.h tests/meta_tests.cpp cx/ast.h tests/parser_ast_tests.cpp cx/regex.h cx/regex_result.h tests/regex_match_tests.cpp cx/char_class.h cx/capture_indexer.h cx/ast_builder.h cx/utility/meta_helpers.h cx/capture_counter.h cx/utility/concepts.h cx/backref_builder.h cx/match_result.h cx/generator.h cx/atomic.h cx/match_context.h cx/capture.h cx/utility/tuple_helpers.h cx/regex_flags.h cx/astfwd.h cx/utility/char_helpers.h cx/match_params.h cx/quantifier_builder.h tests/flag_tests.cpp cx/regex_union.h cx/regex_base.h tests/regex_union_tests.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fconcepts -O3")