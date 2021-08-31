cmake_minimum_required(VERSION 3.19)
project(meta)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        meta
        main.cpp
        meta/grammar/grammar.hpp
        meta/utility/static_string.hpp
        meta/symbol.hpp
        meta/parser.hpp
        meta/utility/stack.hpp
        tests/parser_accept_tests.cpp
        tests/stack_tests.cpp
        tests/tests.hpp
        tests/traits_tests.cpp
        meta/ast/ast.hpp
        tests/parser_ast_tests.cpp
        meta/regex.hpp
        meta/regex_result.hpp
        tests/regex_match_tests.cpp
        tests/regex_search_tests.cpp
        meta/grammar/esc_rules.hpp
        meta/ast/capture_indexer.hpp
        meta/ast/ast_builder.hpp
        meta/utility/meta_helpers.hpp
        meta/utility/concepts.hpp
        meta/grammar/backref_rules.hpp
        meta/ast/match_result.hpp
        meta/iterable_generator_adapter.hpp
        meta/ast/ast_traits.hpp
        meta/regex_context.hpp
        meta/utility/tuple_helpers.hpp
        meta/regex_flags.hpp
        meta/ast/astfwd.hpp
        meta/utility/char_traits.hpp
        meta/grammar/quantifier_rules.hpp
        tests/flag_tests.cpp
        meta/regex_union.hpp
        meta/regex_base.hpp
        tests/regex_union_tests.cpp
        meta/grammar/grammar_actions.hpp
        meta/utility/universal_capture.hpp
        meta/regex_capture.hpp
        meta/regex_token_generator.hpp
        tests/regex_generator_tests.cpp
        meta/utility/continuations.hpp
        meta/ast/terminals.hpp
        meta/ast/sequence.hpp
        meta/ast/alternation.hpp
        meta/ast/disjunction.hpp
        meta/ast/star.hpp
        meta/ast/repetition.hpp
        meta/ast/capturing.hpp
        meta/ast/lookarounds.hpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -O3 -fconcepts-diagnostics-depth=3")