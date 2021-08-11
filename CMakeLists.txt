cmake_minimum_required(VERSION 3.19)
project(meta)

set(CMAKE_CXX_STANDARD 20)

add_executable(meta main.cpp meta/grammar/grammar.hpp meta/utility/static_string.hpp meta/symbol.hpp meta/parser.hpp meta/utility/stack.hpp tests/parser_accept_tests.cpp tests/stack_tests.cpp tests/tests.hpp tests/meta_tests.cpp meta/ast/ast.hpp tests/parser_ast_tests.cpp meta/regex.hpp meta/regex_result.hpp tests/regex_match_tests.cpp meta/grammar/esc_rules.hpp meta/ast/capture_indexer.hpp meta/ast/ast_builder.hpp meta/utility/meta_helpers.hpp meta/ast/capture_counter.hpp meta/utility/concepts.hpp meta/grammar/backref_rules.hpp meta/ast/match_result.hpp meta/generator.hpp meta/ast/ast_traits.hpp meta/match_context.hpp meta/capture.hpp meta/utility/tuple_map.hpp meta/regex_flags.hpp meta/ast/astfwd.hpp meta/utility/char_traits.hpp meta/ast/match_params.hpp meta/grammar/quantifier_rules.hpp tests/flag_tests.cpp meta/regex_union.hpp meta/regex_base.hpp tests/regex_union_tests.cpp meta/ast/atomic_counter.hpp meta/match_context_base.hpp meta/atomic_match_context.hpp meta/grammar/grammar_actions.hpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -fconcepts -O3")