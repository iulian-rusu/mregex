#ifndef MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP
#define MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP

#include <mregex/ast/nodes/terminals/range.hpp>
#include <mregex/ast/nodes/set.hpp>

namespace meta::ast::predicates
{
    template<line_mode Mode>
    struct beginning_predicate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter /*end*/, Iter current, Context &ctx) noexcept
        {
            if (current == begin)
                return true;
            if constexpr (Mode == line_mode::multiline && Context::flags::multiline)
            {
                if (linebreak::match_one(std::prev(current), ctx))
                    return true;
            }
            return false;
        }
    };

    template<line_mode Mode>
    struct end_predicate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter /*begin*/, Iter end, Iter current, Context &ctx) noexcept
        {
            if (current == end)
                return true;
            if constexpr (Mode == line_mode::multiline && Context::flags::multiline)
            {
                if (linebreak::match_one(current, ctx))
                    return true;
            }
            return false;
        }
    };

    struct word_boundary_predicate
    {
        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter current, Context &ctx) noexcept
        {
            bool is_word_behind = current != begin && word::match_one(std::prev(current), ctx);
            bool is_word_ahead = current != end && word::match_one(current, ctx);
            return is_word_behind ^ is_word_ahead;
        }
    };
}
#endif //MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP