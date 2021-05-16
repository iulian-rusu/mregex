#ifndef CTR_MATCHING_H
#define CTR_MATCHING_H

#include <numeric>
#include "parser.h"

namespace ctr
{
    template<static_string pattern>
    constexpr match_result match(std::string_view input)
    {
        static_assert(parser<static_string<pattern.length>(pattern)>::accepted, "invalid regular expression syntax");
        using AST = typename parser<static_string<pattern.length>(pattern)>::ast;

        return AST::match(input, 0, std::numeric_limits<int>::max());
    }
}
#endif //CTR_MATCHING_H
