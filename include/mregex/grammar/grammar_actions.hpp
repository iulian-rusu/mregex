#ifndef MREGEX_GRAMMAR_ACTIONS_HPP
#define MREGEX_GRAMMAR_ACTIONS_HPP

#include <type_traits>

namespace meta::grammar
{
    struct ignore {}; // Ignore the current symbol on top of the stack and continue

    struct advance {}; // Move to the next token in the input sequence

    struct accept : std::true_type {}; // Accept the input sequence

    struct reject : std::false_type {}; // Reject the input sequence
}
#endif //MREGEX_GRAMMAR_ACTIONS_HPP