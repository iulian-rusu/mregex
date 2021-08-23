#ifndef META_REGEX_FLAGS_HPP
#define META_REGEX_FLAGS_HPP

#include "utility/meta_helpers.hpp"

namespace meta
{
    /**
     * Type used for regex flags.
     * Every flag must have an alias value which is defined as its ID xor 1.
     *
     * @tparam ID   The unique identifier of the flag
     */
    template<std::size_t ID>
    struct regex_flag : std::integral_constant<std::size_t, ID>
    {
        static constexpr std::size_t alias_value = ID ^ 1u;
    };

    namespace flag
    {
        /**
         * Makes matching case-insensitive.
         */
        using ignore_case = regex_flag<0x0>;
        using i = regex_flag<0x1>;

        /**
         * Makes wildcards (.) match newline characters.
         * Will make matching faster and generate cleaner assembly.
         */
        using dotall = regex_flag<0x2>;
        using s = regex_flag<0x3>;

        /**
         * Makes anchors ($ and ^) also match beginnings/ends of lines.
         */
        using multiline = regex_flag<0x4>;
        using m = regex_flag<0x5>;

        /**
         * Makes alternations always consume the most characters possible.
         * Might make the performance worse but guarantees finding matches that
         * could otherwise be missed.
         */
        using greedy_alt = regex_flag<0x6>;
        using a = regex_flag<0x7>;

        /**
         * Makes star operators (*) cache intermediate matches (up to a fixed limit).
         * Will result in worse performance for most patterns, unless they
         * require a lot of backtracking on non-trivial subsequences.
         */
        using cache = regex_flag<0x8>;
        using c = regex_flag<0x9>;
    }

    template<typename Flag>
    using alias = regex_flag<Flag::alias_value>;

    template<typename T>
    struct is_flag : std::false_type {};

    template<std::size_t ID>
    struct is_flag<regex_flag<ID>> : std::true_type {};

    template<typename T>
    constexpr bool is_flag_v = is_flag<T>::value;

    // Metafunction to check if a flags has been enabled
    template<typename Flag, typename ... Flags>
    struct is_flag_enabled : std::bool_constant<is_any_of_v<Flag, Flags ...> || is_any_of_v<alias<Flag>, Flags ...>> {};

    template<typename Flag, typename ... Flags>
    constexpr bool is_flag_enabled_v = is_flag_enabled<Flag, Flags ...>::value;
}
#endif //META_REGEX_FLAGS_HPP