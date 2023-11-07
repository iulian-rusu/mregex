#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;

    namespace
    {
        template<typename AST>
        inline constexpr bool is_same_after_indexing = std::is_same_v<ast::index_captures_t<AST>, AST>;

        template<typename AST, typename Expected>
        inline constexpr bool is_indexing_of = std::is_same_v<ast::index_captures_t<AST>, Expected>;

        template<std::size_t ID, typename... Nodes>
        struct make_capture
        {
            using type = unnamed_capture<ID, sequence<Nodes ...>>;
        };

        template<std::size_t ID, typename Node>
        struct make_capture<ID, Node>
        {
            using type = unnamed_capture<ID, Node>;
        };

        template<std::size_t ID>
        struct make_capture<ID>
        {
            using type = unnamed_capture<ID, nothing>;
        };

        template<std::size_t ID, typename... Nodes>
        using id = typename make_capture<ID, Nodes ...>::type;

        template<typename... Nodes>
        using group = typename make_capture<0, Nodes ...>::type;
    }

    static_assert(is_same_after_indexing<nothing>);
    static_assert(is_same_after_indexing<wildcard>);
    static_assert(is_same_after_indexing<literal<'a'>>);
    static_assert(is_same_after_indexing<word>);
    static_assert(is_same_after_indexing<word_boundary>);
    static_assert(is_same_after_indexing<sequence<digit, whitespace>>);
    static_assert(is_same_after_indexing<unnamed_capture<1, wildcard>>);
    static_assert(is_same_after_indexing<sequence<unnamed_capture<1, wildcard>>>);
    static_assert(is_same_after_indexing<alternation<unnamed_capture<1, wildcard>>>);
    static_assert(is_same_after_indexing<positive_lookahead<unnamed_capture<1, wildcard>>>);
    static_assert(is_same_after_indexing<sequence<word, unnamed_capture<1, wildcard>, digit>>);
    static_assert(is_same_after_indexing<sequence<word, star<unnamed_capture<1, wildcard>>, digit>>);

    static_assert(
        is_indexing_of
        <
            group<group<>>,
            id<1, id<2>>
        >
    );
    static_assert(
        is_indexing_of
        <
            sequence<group<>, group<>>,
            sequence<id<1>, id<2>>
        >
    );
    static_assert(
        is_indexing_of
        <
            group<group<group<group<>>>>,
            id<1, id<2, id<3, id<4>>>>
        >
    );
    static_assert(
        is_indexing_of
        <
            sequence<group<group<group<group<>>>>, group<group<group<group<>>>>>,
            sequence<id<1, id<2, id<3, id<4>>>>, id<5, id<6, id<7, id<8>>>>>
        >
    );
    static_assert(
        is_indexing_of
        <
            group<group<>, group<>, group<>>,
            id<1, id<2>, id<3>, id<4>>
        >
    );
    static_assert(
        is_indexing_of
        <
            group<group<group<>, group<>>, group<>, group<>, group<group<>, group<group<>>, group<>>>,
            id<1, id<2, id<3>, id<4>>, id<5>, id<6>, id<7, id<8>, id<9, id<10>>, id<11>>>
        >
    );
}