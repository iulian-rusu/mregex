#ifndef MREGEX_REGEX_MATCH_GENERATOR_HPP
#define MREGEX_REGEX_MATCH_GENERATOR_HPP

#include <mregex/regex_context.hpp>
#include <mregex/regex_methods.hpp>
#include <mregex/regex_traits.hpp>

namespace meta
{
    /**
     * A functor that searches through the range given by an iterator pair and
     * yields all regex matches. At most one empty match will be generated.
     *
     * @tparam Regex    The regex type used to generate matches
     * @tparam Iter     The forward iterator type used to acces the input sequence
     */
    template<typename Regex, std::forward_iterator Iter>
    struct regex_match_generator
    {
        using iterator_type = Iter;
        using ast_type = regex_ast_t<Regex>;
        using context_type = regex_context_t<Regex, iterator_type>;
        using result_view_type = regex_result_view_t<Regex, iterator_type>;
        using continuation_category = continuations<iterator_type>;
        using method = search_method<Regex>;

        constexpr regex_match_generator(iterator_type begin, iterator_type end)
            : _begin{begin}, _end{end}, _current{begin}, _active{true}
        {}

        [[nodiscard]] constexpr result_view_type operator()() noexcept
        {
            context_type ctx{};
            if (_active)
            {
                if (auto result = method::invoke(_begin, _end, _current, ctx))
                {
                    _active = std::get<0>(ctx.captures).length() != 0;
                    _current = result.end;
                    return result_view_type{std::move(ctx.captures), true};
                }
                _active = false;
            }
            return result_view_type{ctx.captures, false};
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