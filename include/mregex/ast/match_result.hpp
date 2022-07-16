#ifndef MREGEX_MATCH_RESULT_HPP
#define MREGEX_MATCH_RESULT_HPP

#include <concepts>

namespace meta::ast
{
    /**
     * Data structure returned by matching methods of AST nodes.
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

        constexpr bool operator==(bool value) const noexcept
        {
            return matched == value;
        }
    };
}
#endif //MREGEX_MATCH_RESULT_HPP