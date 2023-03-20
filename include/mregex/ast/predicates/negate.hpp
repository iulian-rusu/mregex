#ifndef MREGEX_PREDICATES_NEGATE_HPP
#define MREGEX_PREDICATES_NEGATE_HPP

#include <iterator>

namespace meta::ast::predicates
{
    template<typename Predicate>
    struct negate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter current, Context &ctx) noexcept
        {
            return !Predicate::is_match(begin, end, current, ctx);
        }
    };
}
#endif //MREGEX_PREDICATES_NEGATE_HPP