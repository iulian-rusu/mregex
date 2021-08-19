#ifndef META_TESTS_HPP
#define META_TESTS_HPP

#include "../meta/regex.hpp"
#include "../meta/regex_union.hpp"

// Macro defined to run all tests
#define META_RUN_TESTS1

#ifdef META_RUN_TESTS
#define META_RUN_PARSER_TESTS
#define META_RUN_REGEX_TESTS
#define META_RUN_REGEX_UNION_TESTS
#define META_RUN_STACK_TESTS
#define META_RUN_TRAITS_TESTS
#define META_RUN_FLAG_TESTS
#endif

#endif //META_TESTS_HPP