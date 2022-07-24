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
        using continuation_category = continuations<iterator_type>;
        using method = search_method<ast_type>;

        constexpr regex_match_generator(iterator_type begin, iterator_type end)
            : _begin{begin}, _end{end}, _current{begin}, _active{true}
        {}

        [[nodiscard]] constexpr result_type operator()() noexcept
        {
            Context ctx{};
            if (_active)
            {
                if (auto result = method::invoke(_begin, _end, _current, ctx))
                {
                    _active = std::get<0>(ctx.captures).length() != 0;
                    _current = result.end;
                    return result_type{true, std::move(ctx.captures)};
                }
                _active = false;
            }
            return result_type{false, ctx.captures};
        }

        [[nodiscard]] constexpr bool active() const noexcept
        {
            return _active;
        }

    private:
        iterator_type const _begin;
        iterator_type const _end;
        iterator_type _current;
        bool _active;
    };
}
#endif //MREGEX_REGEX_MATCH_GENERATOR_HPP