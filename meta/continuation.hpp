#ifndef META_CONTINUATION_HPP
#define META_CONTINUATION_HPP

#include "ast/match_result.hpp"

namespace meta
{
    /**
     * Struct containing static lambda expressions used as continuations for regex matching.
     * Calling a continuation is equivalent to evaluating the rest of the regex from
     * the calling point forward.
     */
    template<std::forward_iterator Iter>
    struct continuations
    {
        static constexpr auto empty = [](Iter it) -> ast::match_result<Iter> {
            return {it, true};
        };

        static constexpr auto equals = [](Iter target) {
            return [=](Iter it) -> ast::match_result<Iter> {
                return {it, it == target};
            };
        };
    };
}
#endif //META_CONTINUATION_HPP