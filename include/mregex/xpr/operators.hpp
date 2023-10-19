#ifndef MREGEX_XPR_OPERATORS_HPP
#define MREGEX_XPR_OPERATORS_HPP

#include <mregex/xpr/components.hpp>

namespace meta::xpr::operators
{
    template<typename T, typename U>
    constexpr auto operator>>(regex_adapter<T> lhs, regex_adapter<U> rhs) noexcept
    {
        return concat(lhs, rhs);
    }

    template<typename T, typename U>
    constexpr auto operator|(regex_adapter<T> lhs, regex_adapter<U> rhs) noexcept
    {
        return either(lhs, rhs);
    }

    template<typename AST>
    constexpr auto operator!(regex_adapter<AST> expression) noexcept
    {
        return negate(expression);
    }

    template<typename AST>
    constexpr auto operator*(regex_adapter<AST> expression) noexcept
    {
        return zero_or_more(expression);
    }

    template<typename AST>
    constexpr auto operator+(regex_adapter<AST> expression) noexcept
    {
        return one_or_more(expression);
    }
}
#endif //MREGEX_XPR_OPERATORS_HPP