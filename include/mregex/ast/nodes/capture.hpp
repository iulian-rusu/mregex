#ifndef MREGEX_NODES_CAPTURE_HPP
#define MREGEX_NODES_CAPTURE_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/symbols/names.hpp>

namespace meta::ast
{
    /**
     * @note Since matching captures implies side effects like storing and clearing matched
     * content, they cannot be considered trivially matchable even when the inner node is.
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
            using base_iterator = typename Context::iterator;
            using capture_view_type = regex_capture_view<base_iterator, Name>;

            // Iterator types might be different if matching was done inside a lookbehind
            if constexpr (std::is_same_v<Iter, base_iterator>)
                std::get<ID>(ctx.captures) = capture_view_type{begin, end};
            else
                std::get<ID>(ctx.captures) = capture_view_type{end.base(), begin.base()};
        }
    };

    template<std::size_t ID, typename Inner>
    using unnamed_capture = capture<ID, symbol::unnamed, Inner>;
}
#endif //MREGEX_NODES_CAPTURE_HPP