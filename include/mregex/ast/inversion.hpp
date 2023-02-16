#ifndef MREGEX_AST_INVERSION_HPP
#define MREGEX_AST_INVERSION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/traversal.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<typename... Elems>
        constexpr auto to_sequence(type_sequence<Elems ...>) noexcept -> sequence<Elems ...> { return {}; }
    }

    /**
     * Metafuction that inverts (mirrors) the AST at compile-time. This is required to implement
     * lookbehind assertions which need to match a regular expression backwards.
     *
     * @tparam Node The AST node type being inverted
     */
    template<typename Node>
    struct invert : traverse<invert, Node> {};

    template<typename Node>
    using invert_t = typename invert<Node>::type;

    template<typename... Inner>
    struct invert<sequence<Inner ...>>
    {
        using type = decltype(detail::to_sequence(reverse_t<type_sequence<invert_t<Inner> ...>>{}));
    };

    template<typename Inner>
    struct invert<positive_lookahead<Inner>>
    {
        using type = positive_lookbehind<Inner>;
    };

    template<typename Inner>
    struct invert<negative_lookahead<Inner>>
    {
        using type = negative_lookbehind<Inner>;
    };

    template<typename Inner>
    struct invert<positive_lookbehind<Inner>>
    {
        using type = positive_lookahead<Inner>;
    };

    template<typename Inner>
    struct invert<negative_lookbehind<Inner>>
    {
        using type = negative_lookahead<Inner>;
    };
}
#endif //MREGEX_AST_INVERSION_HPP