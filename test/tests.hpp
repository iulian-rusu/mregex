#ifndef MREGEX_TESTS_HPP
#define MREGEX_TESTS_HPP

#include <mregex.hpp>

// Macro defined to run all tests
#define MREGEX_RUN_TESTS

#ifdef MREGEX_RUN_TESTS
#define MREGEX_RUN_PARSER_TESTS
#define MREGEX_RUN_REGEX_TESTS
#define MREGEX_RUN_REGEX_UNION_TESTS
#define MREGEX_RUN_STACK_TESTS
#define MREGEX_RUN_TRAITS_TESTS
#define MREGEX_RUN_FLAG_TESTS
#endif

#endif //MREGEX_TESTS_HPP