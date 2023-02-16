#ifndef MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP
#define MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP

#include <mregex/ast/nodes/terminals/range.hpp>
#include <mregex/ast/nodes/terminals/set.hpp>

namespace meta::ast::predicates
{
    template<line_mode Mode>
    struct beginning_predicate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter, Iter it, Context &ctx) noexcept
        {
            if (it == begin)
                return true;
            if constexpr (Mode == line_mode::multiline && Context::flags::multiline)
            {
                if (linebreak::match_one(std::prev(it), ctx))
                    return true;
            }
            return false;
        }
    };

    template<line_mode Mode>
    struct end_predicate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter, Iter end, Iter it, Context &ctx) noexcept
        {
            if (it == end)
                return true;
            if constexpr (Mode == line_mode::multiline && Context::flags::multiline)
            {
                if (linebreak::match_one(it, ctx))
                    return true;
            }
            return false;
        }
    };

    struct word_boundary_predicate
    {
        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter it, Context &ctx) noexcept
        {
            bool behind = it != begin && word::match_one(std::prev(it), ctx);
            bool ahead = it != end && word::match_one(it, ctx);
            return behind ^ ahead;
        }
    };
}
#endif //MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP