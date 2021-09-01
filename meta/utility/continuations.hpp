#ifndef META_CONTINUATIONS_HPP
#define META_CONTINUATIONS_HPP

#include "../ast/match_result.hpp"

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
        static constexpr auto epsilon = [](Iter it) noexcept -> ast::match_result<Iter> {
            return {it, true};
        };

        static constexpr auto equals = [](Iter target) noexcept {
            return [=](Iter it) -> ast::match_result<Iter> {
                return {it, it == target};
            };
        };
    };
}
#endif //META_CONTINUATIONS_HPP