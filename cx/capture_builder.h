#ifndef CX_CAPTURE_BUILDER_H
#define CX_CAPTURE_BUILDER_H

#include "ast.h"

/**
 * File with metafunctions for correctly numbering capturing groups
 */
namespace cx
{
    template<auto Offset, typename Wrapper>
    struct renumber_captures
    {
        using type = Wrapper;
    };

    template<auto Offset, typename Wrapper>
    using renumber_captures_t = typename renumber_captures<Offset, Wrapper>::type;

    template<auto Offset, template<typename ...> typename Wrapper,  typename ... Inner>
    struct renumber_captures<Offset, Wrapper<Inner ...>>
    {
        using type = Wrapper<renumber_captures_t<Offset, Inner> ...>;
    };

    template<auto Offset, typename ... Rest>
    struct renumber_captures<Offset, sequence<Rest ...>>
    {
        using type = sequence<renumber_captures_t<Offset, Rest> ...>;
    };

    template<auto Offset, auto N, typename Inner>
    struct renumber_captures<Offset, capturing<N, Inner>>
    {
        static constexpr auto capture_count = Inner::capture_count;
        using type = capturing<N + Offset - capture_count, renumber_captures_t<Offset + 1, Inner>>;
    };
}
#endif //CX_CAPTURE_BUILDER_H
