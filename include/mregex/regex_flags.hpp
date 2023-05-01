#ifndef MREGEX_REGEX_FLAGS_HPP
#define MREGEX_REGEX_FLAGS_HPP

#include <type_traits>

namespace meta
{
    /**
     * Type used for regex flags.
     *
     * @tparam ID   The unique identifier of the flag
     */
    template<std::size_t ID>
    struct regex_flag : std::integral_constant<std::size_t, ID> {};

    namespace flag
    {
        /**
         * Makes matching case-insensitive.
         */
        using icase = regex_flag<0x0>;
        using i = icase;

        /**
         * Makes wildcards (.) match line break characters.
         */
        using dotall = regex_flag<0x1>;
        using s = dotall;

        /**
         * Makes anchors ($ and ^) also match the beginning/end of lines.
         */
        using multiline = regex_flag<0x2>;
        using m = multiline;

        /**
         * Swaps the default syntax for lazy and greedy quantifers.
         */
        using ungreedy = regex_flag<0x3>;
        using U = ungreedy;

        /**
         * Enables the compiler to be more aggressive when unrolling repetition loops.
         */
        using unroll = regex_flag<0x4>;
        using r = unroll;
    }

    template<typename T>
    inline constexpr bool is_flag = false;

    template<std::size_t ID>
    inline constexpr bool is_flag<regex_flag<ID>> = true;

    template<typename Flag, typename... Flags>
    inline constexpr bool is_flag_enabled = (std::is_same_v<Flag, Flags> || ...);

    template<typename... Flags>
    struct regex_flag_accessor
    {
        static_assert((is_flag<Flags> && ...), "invalid flag");

        static constexpr bool icase = is_flag_enabled<flag::icase, Flags ...>;
        static constexpr bool dotall = is_flag_enabled<flag::dotall, Flags ...>;
        static constexpr bool multiline = is_flag_enabled<flag::multiline, Flags ...>;
        static constexpr bool ungreedy = is_flag_enabled<flag::ungreedy, Flags ...>;
        static constexpr bool unroll = is_flag_enabled<flag::unroll, Flags ...>;
    };
}
#endif //MREGEX_REGEX_FLAGS_HPP