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

        constexpr match_result operator+(match_result const &other) const noexcept
        {
            return match_result{other.end, matched || other.matched};
        }

        constexpr match_result &operator+=(match_result const &other) noexcept
        {
            end = other.end;
            matched = matched || other.matched;
            return *this;
        }
    };
}
#endif //MREGEX_MATCH_RESULT_HPP