#ifndef MREGEX_NODES_CAPTURE_HPP
#define MREGEX_NODES_CAPTURE_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/symbols/names.hpp>

namespace meta::ast
{
    template<std::size_t ID, typename Name, typename Inner>
    struct capture
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter next) noexcept {
                capture_matched_range(current, next, ctx);
                return cont(next);
            };
            if (auto inner_match = Inner::match(begin, end, current, ctx, continuation))
                return inner_match;
            std::get<ID>(ctx.captures).clear();
            return {current, false};
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