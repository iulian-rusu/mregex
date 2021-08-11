#ifndef META_GRAMMAR_ACTIONS_HPP
#define META_GRAMMAR_ACTIONS_HPP

#include <type_traits>

namespace meta::grammar
{
    struct ignore {}; // Ignore the current symbol on top of the stack and continue parsing

    struct advance {}; // Move to the next character in the parser input

    struct accept : std::true_type {}; // Accept the parser input

    struct reject : std::false_type {}; // Reject the parser input
}
#endif //META_GRAMMAR_ACTIONS_HPP