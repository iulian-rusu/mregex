#ifndef MREGEX_REGEX_FLAGS_HPP
#define MREGEX_REGEX_FLAGS_HPP

#include <mregex/utility/type_traits.hpp>

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
         * Makes wildcards (.) match newline characters.
         */
        using dotall = regex_flag<0x1>;
        using s = dotall;

        /**
         * Makes anchors ($ and ^) also match beginnings/ends of lines.
         */
        using multiline = regex_flag<0x2>;
        using m = multiline;

        /**
         * Makes the Kleene star operator (*) consume as few elements as possible.
         */
        using ungreedy = regex_flag<0x3>;
        using U = ungreedy;

        /**
         * Makes the compiler unroll even non-trivial repetition loops.
         */
        using unroll = regex_flag<0x4>;
        using r = unroll;
    }

    template<typename>
    inline constexpr bool is_flag_v = false;

    template<std::size_t ID>
    inline constexpr bool is_flag_v<regex_flag<ID>> = true;

    template<typename Flag, typename... Flags>
    inline constexpr bool is_flag_enabled_v = is_any_of_v<Flag, Flags ...>;
}
#endif //MREGEX_REGEX_FLAGS_HPP