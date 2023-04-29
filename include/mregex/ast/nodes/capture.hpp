#ifndef MREGEX_NODES_CAPTURE_HPP
#define MREGEX_NODES_CAPTURE_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/symbols/names.hpp>

namespace meta::ast
{
    /**
     * @note Captures cannot be considered trivially matchable even when the inner node is.
     * This is because trivially matchable nodes do not have access to continuation functions when matching,
     * making it impossible for the capture to unmatch when a subsequent AST node fails to match.
     */
    template<std::size_t ID, typename Name, typename Inner>
    struct capture
    {
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                capture_matched_range(current, next, ctx);
                return cont(next);
            };
            if (auto inner_match = Inner::match(begin, end, current, ctx, continuation))
                return inner_match;
            std::get<ID>(ctx.captures).clear();
            return non_match(current);
        }

    private:
        template<std::forward_iterator Iter, typename Context>
        static constexpr auto capture_matched_range(Iter begin, Iter end, Context &ctx) noexcept
        {
            using regex_type = typename Context::regex_type;
            using base_iterator_type = typename Context::iterator_type;
            using capture_view_type = regex_capture_view_t<regex_type, ID, base_iterator_type>;

            // Iterator types might be different if matching was done inside a lookbehind
            if constexpr (std::is_same_v<Iter, base_iterator_type>)
                std::get<ID>(ctx.captures) = capture_view_type{begin, end};
            else
                std::get<ID>(ctx.captures) = capture_view_type{end.base(), begin.base()};
        }
    };

    template<std::size_t ID, typename Inner>
    using unnamed_capture = capture<ID, symbol::unnamed, Inner>;
}
#endif //MREGEX_NODES_CAPTURE_HPP