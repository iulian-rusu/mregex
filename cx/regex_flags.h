#ifndef CX_REGEX_FLAGS_H
#define CX_REGEX_FLAGS_H

#include "utility/meta_helpers.h"

namespace cx
{
    /**
     * Type used for Regex flags.
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
        // Enables case-insensitive matching
        using ignore_case = regex_flag<0x0>;
        using i = regex_flag<0x1>;

        // Makes wildcard '.' match anything, even newline
        using dotall = regex_flag<0x2>;
        using s = regex_flag<0x3>;

        // Makes anchors like '^' and '$' also match line beginning/ending
        using multiline = regex_flag<0x4>;
        using m = regex_flag<0x5>;

        // Ignores any space characters in the Regex
        using extended = regex_flag<0x6>;
        using x = regex_flag<0x7>;

        // Makes alternation always match the longest substring (custom flag)
        using greedy_alt = regex_flag<0x8>;
        using a = regex_flag<0x9>;
    }

    template<typename Flag>
    using alias = regex_flag<Flag::alias_value>;

    // Type trait to check if a type is a valid Regex flag
    template<typename T>
    struct is_flag : std::false_type {};

    template<std::size_t ID>
    struct is_flag<regex_flag<ID>> : std::true_type {};

    template<typename T>
    constexpr bool is_flag_v = is_flag<T>::value;

    // Type trait to check if a flags has been set
    template<typename Flag, typename ... Flags>
    struct check_flag : std::bool_constant<is_any_of_v<Flag, Flags ...> || is_any_of_v<alias<Flag>, Flags ...>> {};

    template<typename Flag, typename ... Flags>
    constexpr bool check_flag_v = check_flag<Flag, Flags ...>::value;
}
#endif //CX_REGEX_FLAGS_H