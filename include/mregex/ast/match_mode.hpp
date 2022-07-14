#ifndef MREGEX_MATCH_MODE_HPP
#define MREGEX_MATCH_MODE_HPP

#include <cstdint>

namespace meta::ast
{
    /**
     * Enumerator which lists the possible match modes
     * for various regex quantifiers.
     */
    enum class match_mode : std::int8_t
    {
        greedy = 0,
        lazy,
        possessive
    };
}
#endif //MREGEX_MATCH_MODE_HPP