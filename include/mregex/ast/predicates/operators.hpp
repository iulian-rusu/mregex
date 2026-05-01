#ifndef MREGEX_PREDICATES_OPERATORS_HPP
#define MREGEX_PREDICATES_OPERATORS_HPP

#include <iterator>

namespace meta::ast::predicates
{
    template<typename P>
    struct negate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter current, Context &ctx) noexcept
        {
            return !P::is_match(begin, end, current, ctx);
        }
    };

    template<typename P, typename Q>
    struct logical_or
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter current, Context &ctx) noexcept
        {
            return P::is_match(begin, end, current, ctx) || Q::is_match(begin, end, current, ctx);
        }
    };

    template<typename P, typename Q>
    struct exclusive_or
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter current, Context &ctx) noexcept
        {
            return P::is_match(begin, end, current, ctx) ^ Q::is_match(begin, end, current, ctx);
        }
    };
}
#endif //MREGEX_PREDICATES_OPERATORS_HPP