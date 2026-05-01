#ifndef MREGEX_UTILITY_ENUMS_HPP
#define MREGEX_UTILITY_ENUMS_HPP

#include <cstdint>

namespace meta
{
    /**
     * Match modes for various regex quantifiers.
     */
    enum struct match_mode : std::uint8_t
    {
        greedy,
        lazy,
        possessive
    };

    /**
     * Directions in which lookaround assertions can match.
     */
    enum struct lookaround_direction : std::uint8_t
    {
        ahead,
        behind
    };
}
#endif //MREGEX_UTILITY_ENUMS_HPP