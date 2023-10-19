#ifndef MREGEX_MATCH_RESULT_GENERATOR_HPP
#define MREGEX_MATCH_RESULT_GENERATOR_HPP

#include <mregex/regex_match_context.hpp>
#include <mregex/regex_methods.hpp>

namespace meta
{
    /**
     * A functor that generates regex results by iterating a forward range.
     * At most one empty result will be generated.
     *
     * @tparam Method   The regex method used to generate results
     * @tparam Iter     The forward iterator type used to access the input
     */
    template<typename Method, std::forward_iterator Iter>
    struct match_result_generator
    {
        using method = Method;
        using iterator = Iter;
        using regex_type = typename method::regex_type;
        using ast_type = regex_ast_t<regex_type>;

        constexpr match_result_generator(iterator begin, iterator end)
            : _begin{begin}, _end{end}, _current{begin}
        {}

        [[nodiscard]] constexpr auto operator()() noexcept
        {
            return next();
        }

        [[nodiscard]] constexpr auto next() noexcept
        {
            regex_match_context<regex_type, iterator> ctx{};
            if (!_active)
                return match_result_view<regex_type, iterator>{std::move(ctx.captures), false};

            auto result = method::invoke(_begin, _end, _current, ctx);
            _active = result.matched && !std::get<0>(ctx.captures).is_empty();
            _current = result.end;
            return match_result_view<regex_type, iterator>{std::move(ctx.captures), result.matched};
        }

        constexpr bool active() const noexcept
        {
            return _active;
        }

    private:
        iterator _begin;
        iterator _end;
        iterator _current;
        bool _active{true};
    };
}
#endif //MREGEX_MATCH_RESULT_GENERATOR_HPP