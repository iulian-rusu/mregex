#ifndef CX_TESTS_H
#define CX_TESTS_H

#include "../cxregex.h"

// macro defined to run all tests
#define CX_RUN_TESTS

#ifdef CX_RUN_TESTS
#define CX_RUN_PARSER_TESTS
#define CX_RUN_STACK_TESTS
#define CX_RUN_UTILITY_TESTS
#endif

#endif //CX_TESTS_H
