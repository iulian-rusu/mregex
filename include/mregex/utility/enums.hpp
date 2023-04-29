#ifndef MREGEX_UTILITY_ENUMS_HPP
#define MREGEX_UTILITY_ENUMS_HPP

namespace meta
{
    /**
     * Enumerator which lists the possible match modes
     * for various regex quantifiers.
     */
    enum struct match_mode
    {
        greedy,
        lazy,
        possessive
    };

    /**
     * Enumerator which lists line sensitivity modes
     * for matching regex patterns.
     */
    enum struct line_mode
    {
        singleline,
        multiline
    };

    /**
     * Enumerator which lists the possible directions in which
     * lookaround assertions can match.
     */
    enum struct lookaround_direction
    {
        ahead,
        behind
    };
}
#endif //MREGEX_UTILITY_ENUMS_HPP