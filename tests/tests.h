#ifndef CX_TESTS_H
#define CX_TESTS_H

#include "../cxregex.h"

// Macro defined to run all tests
#define CX_RUN_TESTS

#ifdef CX_RUN_TESTS
#define CX_RUN_PARSER_TESTS
#define CX_RUN_REGEX_TESTS
#define CX_RUN_REGEX_UNION_TESTS
#define CX_RUN_STACK_TESTS
#define CX_RUN_UTILITY_TESTS
#define CX_RUN_FLAG_TESTS
#endif

#endif //CX_TESTS_H
