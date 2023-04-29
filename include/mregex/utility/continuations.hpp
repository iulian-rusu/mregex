#ifndef MREGEX_UTILITY_CONTINUATIONS_HPP
#define MREGEX_UTILITY_CONTINUATIONS_HPP

#include <mregex/ast/match_result.hpp>

namespace meta
{
    template<typename Cont, typename Iter>
    concept match_continuation = std::forward_iterator<Iter> && requires (Cont cont, Iter current)
    {
        { cont(current) } -> std::same_as<ast::match_result<Iter>>;
    };

    /**
     * Struct containing static lambda expressions used as continuations for regex matching.
     * Calling a continuation is equivalent to evaluating the rest of the regex from
     * the calling point onwards.
     */
    template<std::forward_iterator Iter>
    struct continuations
    {
        static constexpr auto success = [](Iter next) noexcept -> ast::match_result<Iter> {
            return {next, true};
        };

        static constexpr auto equals = [](Iter target) noexcept {
            return [=](Iter next) noexcept -> ast::match_result<Iter> {
                return {next, next == target};
            };
        };
    };
}
#endif //MREGEX_UTILITY_CONTINUATIONS_HPP