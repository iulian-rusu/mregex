#ifndef MREGEX_AST_INVERSION_HPP
#define MREGEX_AST_INVERSION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/transform.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<typename... Elems>
        constexpr auto to_sequence(type_sequence<Elems ...>) noexcept -> sequence<Elems ...> { return {}; }
    }

    /**
     * Metafunction that inverts (mirrors) the AST at compile-time. This is required to implement
     * lookbehind assertions which need to match a regular expression backwards.
     *
     * @tparam Node The AST node type being inverted
     */
    template<typename Node>
    struct invert : transform<Node, invert> {};

    template<typename Node>
    using invert_t = typename invert<Node>::type;

    template<typename... Nodes>
    struct invert<sequence<Nodes ...>>
    {
        using type = decltype(detail::to_sequence(reverse_t<type_sequence<invert_t<Nodes> ...>>{}));
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