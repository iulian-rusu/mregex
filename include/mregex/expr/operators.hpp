#ifndef MREGEX_EXPR_OPERATORS_HPP
#define MREGEX_EXPR_OPERATORS_HPP

#include <mregex/expr/components.hpp>
#include <mregex/expr/flags.hpp>

namespace meta::expr::operators
{
    // Operators for defining the AST
    template<typename T, typename U>
    constexpr auto operator>>(regex_interface<T> lhs, regex_interface<U> rhs) noexcept
    {
        return concat(lhs, rhs);
    }

    template<typename T, typename U>
    constexpr auto operator|(regex_interface<T> lhs, regex_interface<U> rhs) noexcept
    {
        return either(lhs, rhs);
    }

    template<typename Node>
    constexpr auto operator!(regex_interface<Node> node) noexcept
    {
        return neg(node);
    }

    template<typename Node>
    constexpr auto operator*(regex_interface<Node> node) noexcept
    {
        return zero_or_more(node);
    }

    template<typename Node>
    constexpr auto operator+(regex_interface<Node> node) noexcept
    {
        return one_or_more(node);
    }

    // Operators for adding flags
    template<typename... Ts, typename Flag>
    constexpr auto operator/(regex_interface<Ts ...>, Flag) noexcept
    {
        return regex_interface<Ts ..., Flag>{};
    }
}
#endif //MREGEX_EXPR_OPERATORS_HPP