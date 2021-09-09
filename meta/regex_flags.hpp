#ifndef META_REGEX_FLAGS_HPP
#define META_REGEX_FLAGS_HPP

#include "utility/meta_helpers.hpp"

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
         * Will make matching faster and generate cleaner assembly.
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
    }

    template<typename T>
    struct is_flag : std::false_type {};

    template<std::size_t ID>
    struct is_flag<regex_flag<ID>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_flag_v = is_flag<T>::value;

    template<typename Flag, typename... Flags>
    struct is_flag_enabled : std::bool_constant<is_any_of_v<Flag, Flags ...>> {};

    template<typename Flag, typename... Flags>
    inline constexpr bool is_flag_enabled_v = is_flag_enabled<Flag, Flags ...>::value;
}
#endif //META_REGEX_FLAGS_HPP