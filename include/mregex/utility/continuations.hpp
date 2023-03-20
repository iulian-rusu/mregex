#ifndef MREGEX_UTILITY_CONTINUATIONS_HPP
#define MREGEX_UTILITY_CONTINUATIONS_HPP

#include <mregex/ast/match_result.hpp>

namespace meta
{
    /**
     * Struct containing static lambda expressions used as continuations for regex matching.
     * Calling a continuation is equivalent to evaluating the rest of the regex from
     * the calling point onwards.
     */
    template<std::forward_iterator Iter>
    struct continuations
    {
        static constexpr auto success = [](Iter current) noexcept -> ast::match_result<Iter> {
            return {current, true};
        };

        static constexpr auto equals = [](Iter target) noexcept {
            return [=](Iter current) noexcept -> ast::match_result<Iter> {
                return {current, current == target};
            };
        };
    };
}
#endif //MREGEX_UTILITY_CONTINUATIONS_HPP