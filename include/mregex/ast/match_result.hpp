#ifndef MREGEX_MATCH_RESULT_HPP
#define MREGEX_MATCH_RESULT_HPP

#include <concepts>

namespace meta::ast
{
    /**
     * Data structure returned by all matching functions of AST nodes.
     */
    template<std::forward_iterator Iter>
    struct match_result
    {
        Iter end{};
        bool matched{};

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }
    };
}
#endif //MREGEX_MATCH_RESULT_HPP