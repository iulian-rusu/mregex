#ifndef MREGEX_GRAMMAR_ACTIONS_HPP
#define MREGEX_GRAMMAR_ACTIONS_HPP

#include <type_traits>

namespace meta::grammar
{
    // Ignore the current symbol at the front of the stack and continue
    struct ignore {};

    // Move to the next token in the input sequence
    struct advance {};

    // Accept the input sequence
    struct accept : std::true_type {};

    // Reject the input sequence
    struct reject : std::false_type {};
}
#endif //MREGEX_GRAMMAR_ACTIONS_HPP