#ifndef MREGEX_REGEX_FLAGS_HPP
#define MREGEX_REGEX_FLAGS_HPP

#include <type_traits>

namespace meta
{
    enum struct regex_flag
    {
        // Makes matching case-insensitive
        icase,
        // Makes wildcards (.) match line break characters
        dotall,
        // Makes anchors ($ and ^) also match the beginning/end of lines
        multiline,
        // Swaps the behavior of lazy and greedy quantifiers
        ungreedy,
        // Enables the compiler to be more aggressive when unrolling repetition loops
        unroll
    };

    /**
     * Metacontainer for a sequence of flags.
     */
    template<regex_flag... Flags>
    struct regex_flag_sequence
    {
        template<regex_flag Flag>
        static constexpr bool test = ((Flag == Flags) || ...);

        static constexpr bool icase = test<regex_flag::icase>;
        static constexpr bool dotall = test<regex_flag::dotall>;
        static constexpr bool multiline = test<regex_flag::multiline>;
        static constexpr bool ungreedy = test<regex_flag::ungreedy>;
        static constexpr bool unroll = test<regex_flag::unroll>;
    };
}
#endif //MREGEX_REGEX_FLAGS_HPP