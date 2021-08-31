#ifndef META_CAPTURING_HPP
#define META_CAPTURING_HPP

#include "astfwd.hpp"
#include "ast_traits.hpp"
#include "match_result.hpp"
#include "../regex_context.hpp"

namespace meta::ast
{
    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                std::get<ID>(ctx.captures) = regex_capture_view<ID, Iter>{it, new_it};
                return cont(new_it);
            };
            return Inner::match(begin, end, it, ctx, continuation);
        }
    };
}
#endif //META_CAPTURING_HPP
