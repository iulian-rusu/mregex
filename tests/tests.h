#ifndef CTR_TESTS_H
#define CTR_TESTS_H

#include "../ctr.h"

// macro defined to run all tests
#define CTR_RUN_TESTS

#ifdef CTR_RUN_TESTS
#define CTR_RUN_PARSER_TESTS
#define CTR_RUN_STACK_TESTS
#endif

#endif //CTR_TESTS_H
