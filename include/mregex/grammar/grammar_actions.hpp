#ifndef MREGEX_GRAMMAR_ACTIONS_HPP
#define MREGEX_GRAMMAR_ACTIONS_HPP

#include <type_traits>

namespace meta::grammar
{
    struct ignore {}; // Ignore the current symbol on top of the stack and continue parsing

    struct advance {}; // Move to the next token in the parser input

    struct accept : std::true_type {}; // Accept the parser input

    struct reject : std::false_type {}; // Reject the parser input
}
#endif //MREGEX_GRAMMAR_ACTIONS_HPP