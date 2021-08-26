#ifndef META_REGEX_TOKEN_GENERATOR_HPP
#define META_REGEX_TOKEN_GENERATOR_HPP

#include <concepts>
#include <iterator>
#include "ast/match_bounds.hpp"
#include "context/match_context.hpp"
#include "regex_result.hpp"

namespace meta
{
    /**
     * A functor that searches through the range given by an iterator pair and
     * yields all non-empty tokens that match a regex.
     *
     * @tparam Context  The matching context formed from the AST of the regular expression
     */
    template<typename Context>
    struct regex_token_generator
    {
        using ast_type = typename Context::ast_type;
        using iterator_type = typename Context::iterator_type;
        using result_type = regex_result_view<ast_type::capture_count, iterator_type>;

        constexpr regex_token_generator(iterator_type b, iterator_type e, iterator_type c)
            : begin{b}, end{e}, bounds{c, static_cast<std::size_t>(std::distance(c, e))}, active{true}
        {}

        [[nodiscard]] constexpr result_type operator()() noexcept
        {
            Context ctx{};
            while (active)
            {
                auto res = ast_type::match(begin, end, bounds, ctx);
                if (res && res.consumed > 0)
                {
                    auto match_begin = bounds.from;
                    bounds = bounds.advance(res.consumed);
                    std::get<0>(ctx.captures) = regex_capture_view<0, iterator_type>{match_begin, bounds.from};
                    active = bounds.from != end;
                    return result_type{true, std::move(ctx.captures)};
                }

                if (bounds.from != end)
                {
                    bounds = bounds.advance(1);
                    ctx.clear();
                }
                else
                {
                    active = false;
                }
            }
            return result_type{false, std::move(ctx.captures)};
        }

    private:
        iterator_type begin;
        iterator_type end;
        ast::match_bounds<iterator_type> bounds;
        bool active;
    };
}
#endif //META_REGEX_TOKEN_GENERATOR_HPP
