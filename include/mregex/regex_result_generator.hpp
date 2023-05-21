#ifndef MREGEX_REGEX_RESULT_GENERATOR_HPP
#define MREGEX_REGEX_RESULT_GENERATOR_HPP

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
    struct regex_result_generator
    {
        using method = Method;
        using iterator_type = Iter;
        using regex_type = typename method::regex_type;
        using ast_type = regex_ast_t<regex_type>;
        using result_view_type = regex_result_view_t<regex_type, iterator_type>;

        constexpr regex_result_generator(iterator_type begin, iterator_type end)
            : _begin{begin}, _end{end}, _current{begin}, _active{true}
        {}

        [[nodiscard]] constexpr result_view_type operator()() noexcept
        {
            return next();
        }

        [[nodiscard]] constexpr result_view_type next() noexcept
        {
            regex_context<regex_type, iterator_type> ctx{};
            if (!_active)
                return result_view_type{std::move(ctx.captures), false};

            auto result = method::invoke(_begin, _end, _current, ctx);
            _active = result.matched && !std::get<0>(ctx.captures).is_empty();
            _current = result.end;
            return result_view_type{std::move(ctx.captures), result.matched};
        }

        constexpr bool active() const noexcept
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
#endif //MREGEX_REGEX_RESULT_GENERATOR_HPP