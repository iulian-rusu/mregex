#ifndef MREGEX_REGEX_MATCH_GENERATOR_HPP
#define MREGEX_REGEX_MATCH_GENERATOR_HPP

#include <mregex/regex_context.hpp>
#include <mregex/regex_methods.hpp>

namespace meta
{
    /**
     * A functor that searches through the range given by an iterator pair and
     * yields all regex matches. At most one empty match will be generated.
     *
     * @tparam Context  The matching context formed from the AST of the regular expression
     */
    template<typename Context>
    struct regex_match_generator
    {
        using ast_type = typename Context::ast_type;
        using iterator_type = typename Context::iterator_type;
        using result_type = typename Context::result_type;
        using capture_type = typename result_type::capture_type;
        using continuation_category = continuations<iterator_type>;
        using method = search_method<ast_type>;

        constexpr regex_match_generator(iterator_type start, iterator_type stop)
            : begin_iter{start}, end_iter{stop}, current_iter{start}, active{true}
        {}

        [[nodiscard]] constexpr result_type operator()() noexcept
        {
            if (active)
            {
                Context ctx{};
                if (auto res = method::invoke(begin_iter, end_iter, current_iter, ctx))
                {
                    active = res.end != current_iter;
                    current_iter = res.end;
                    return result_type{true, std::move(ctx.captures)};
                }
                active = false;
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