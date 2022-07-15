#ifndef MREGEX_REGEX_MATCH_GENERATOR_HPP
#define MREGEX_REGEX_MATCH_GENERATOR_HPP

#include <mregex/utility/continuations.hpp>
#include <mregex/regex_context.hpp>
#include <mregex/regex_result.hpp>

namespace meta
{
    /**
     * A functor that searches through the range given by an iterator pair and
     * yields all non-empty matches of the regex.
     *
     * @tparam Context  The matching context formed from the AST of the regular expression
     */
    template<typename Context>
    struct regex_match_generator
    {
        using ast_type = typename Context::ast_type;
        using iterator_type = typename Context::iterator_type;
        using result_type = typename Context::result_type;
        using continuation_category = continuations<iterator_type>;

        constexpr regex_match_generator(iterator_type start, iterator_type stop)
                : begin_iter{start}, end_iter{stop}, current_iter{start}, active{true}
        {}

        [[nodiscard]] constexpr result_type operator()() noexcept
        {
            while (active)
            {
                Context ctx{};
                auto res = ast_type::match(begin_iter, end_iter, current_iter, ctx, continuation_category::epsilon);
                if (res.matched && res.end != current_iter)
                {
                    auto match_begin = current_iter;
                    std::get<0>(ctx.captures) = regex_capture_view<iterator_type>{match_begin, res.end};
                    current_iter = res.end;
                    active = current_iter != end_iter;
                    return result_type{true, std::move(ctx.captures)};
                }
                active = current_iter++ != end_iter;
            }
            return result_type{false, Context{}.captures};
        }

    private:
        iterator_type const begin_iter;
        iterator_type const end_iter;
        iterator_type current_iter;
        bool active;
    };
}
#endif //MREGEX_REGEX_MATCH_GENERATOR_HPP