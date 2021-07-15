#ifndef CX_REGEX_FLAGS_H
#define CX_REGEX_FLAGS_H

#include "utility/meta_helpers.h"

namespace cx
{
    /**
     * Namespace with some PCRE flags and some custom ones
     */
    namespace flag
    {
        // Enable case-insensitive matching
        struct ignore_case;
        struct i;

        // Make wildcard '.' match anything, even newline
        struct dotall;
        struct s;

        // Make anchors like '^' and '$' also match line beginning/ending
        struct multiline;
        struct m;

        // Ignore any space characters in the Regex
        struct extended;
        struct x;

        // Make alternation always match the longest substring (custom flag)
        struct greedy_alt;
        struct a;

        // Define alias rules below

        struct ignore_case
        {
            using alias = i;
        };

        struct i
        {
            using alias = ignore_case;
        };

        struct dotall
        {
            using alias = s;
        };

        struct s
        {
            using alias = dotall;
        };

        struct multiline
        {
            using alias = m;
        };

        struct m
        {
            using alias = multiline;
        };

        struct extended
        {
            using alias = x;
        };

        struct x
        {
            using alias = extended;
        };

        struct greedy_alt
        {
            using alias = a;
        };

        struct a
        {
            using alias = greedy_alt;
        };
    }

    template<typename Flag>
    using alias = typename Flag::alias;

    template<typename Flag, typename ... Flags>
    struct check_flag : std::bool_constant<is_any_of_v<Flag, Flags ...> || is_any_of_v<alias<Flag>, Flags ...>> {};

    template<typename Flag, typename ... Flags>
    constexpr bool check_flag_v = check_flag<Flag, Flags ...>::value;

    template<typename Context>
    using flags = typename Context::flags;
}
#endif //CX_REGEX_FLAGS_H
