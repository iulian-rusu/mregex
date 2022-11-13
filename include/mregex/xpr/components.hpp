#ifndef MREGEX_XPR_COMPONENTS_HPP
#define MREGEX_XPR_COMPONENTS_HPP

#include <mregex/ast/traits.hpp>
#include <mregex/xpr/adapters.hpp>
#include <mregex/xpr/providers.hpp>
#include <mregex/regex.hpp>

namespace meta::xpr
{
    /**
     * Expression-based API for constructing a regular expression.
     */

    // Sequences
    template<typename... Nodes>
    constexpr auto concat(regex_interface<Nodes>...) noexcept
    {
        return to_regex(flat_wrap<ast::sequence>(Nodes{} ...));
    }

    // Alternations
    template<typename... Nodes>
    constexpr auto either(regex_interface<Nodes>...) noexcept
    {
        return to_regex(flat_wrap<ast::alternation>(Nodes{} ...));
    }

    // Captures
    template<std::size_t ID, static_string Name, typename... Nodes>
    constexpr auto capture(regex_interface<Nodes>...) noexcept
    {
        using wrapper_provider = capture_provider<ID, symbol::name<Name>>;
        return to_regex(pack<wrapper_provider::template type>(Nodes{} ...));
    }

    template<std::size_t ID, typename... Nodes>
    constexpr auto capture(regex_interface<Nodes>...) noexcept
    {
        using wrapper_provider = capture_provider<ID, symbol::unnamed>;
        return to_regex(pack<wrapper_provider::template type>(Nodes{} ...));
    }

    // Repetition
    template<std::size_t A, std::size_t B, match_mode Mode, typename... Nodes>
    constexpr auto between(regex_interface<Nodes>...) noexcept
    {
        using wrapper_provider = repetition_provider<Mode, symbol::quantifier_value<A>, symbol::quantifier_value<B>>;
        return to_regex(pack<wrapper_provider::template type>(Nodes{} ...));
    }

    template<std::size_t A, std::size_t B, typename... Nodes>
    constexpr auto between(regex_interface<Nodes>... nodes) noexcept
    {
        return between<A, B, match_mode::greedy>(nodes ...);
    }

    template<std::size_t N, match_mode Mode, typename... Nodes>
    constexpr auto at_least(regex_interface<Nodes>...) noexcept
    {
        using wrapper_provider = repetition_provider<Mode, symbol::quantifier_value<N>, symbol::quantifier_inf>;
        return to_regex(pack<wrapper_provider::template type>(Nodes{} ...));
    }

    template<std::size_t N, typename... Nodes>
    constexpr auto at_least(regex_interface<Nodes>... nodes) noexcept
    {
        return at_least<N, match_mode::greedy>(nodes ...);
    }

    // Fixed repetition
    template<std::size_t N, typename... Nodes>
    constexpr auto exactly(regex_interface<Nodes>... nodes) noexcept
    {
        return between<N, N>(nodes ...);
    }

    template<std::size_t N, match_mode Mode, typename... Nodes>
    constexpr auto exactly(regex_interface<Nodes>... nodes) noexcept
    {
        return between<N, N, Mode>(nodes ...);
    }

    // Kleene star
    template<match_mode Mode, typename... Nodes>
    constexpr auto zero_or_more(regex_interface<Nodes>... nodes) noexcept
    {
        return at_least<0, Mode>(nodes ...);
    }

    template<typename... Nodes>
    constexpr auto zero_or_more(regex_interface<Nodes>... nodes) noexcept
    {
        return at_least<0>(nodes ...);
    }

    // Plus
    template<match_mode Mode, typename... Nodes>
    constexpr auto one_or_more(regex_interface<Nodes>... nodes) noexcept
    {
        return at_least<1, Mode>(nodes ...);
    }

    template<typename... Nodes>
    constexpr auto one_or_more(regex_interface<Nodes>... nodes) noexcept
    {
        return at_least<1>(nodes ...);
    }

    // Optional
    template<match_mode Mode, typename... Nodes>
    constexpr auto maybe(regex_interface<Nodes>... nodes) noexcept
    {
        return between<0, 1, Mode>(nodes ...);
    }

    template<typename... Nodes>
    constexpr auto maybe(regex_interface<Nodes>... nodes) noexcept
    {
        return between<0, 1, match_mode::greedy>(nodes ...);
    }

    // Terminals
    inline constexpr auto empty = regex_interface<ast::empty>{};
    inline constexpr auto nothing = regex_interface<ast::nothing>{};
    inline constexpr auto begin_line = regex_interface<ast::beginning_of_line>{};
    inline constexpr auto end_line = regex_interface<ast::end_of_line>{};
    inline constexpr auto begin_input = regex_interface<ast::beginning_of_input>{};
    inline constexpr auto end_input = regex_interface<ast::end_of_input>{};
    inline constexpr auto boundary = regex_interface<ast::word_boundary>{};
    inline constexpr auto whitespace = regex_interface<ast::whitespace>{};
    inline constexpr auto any = regex_interface<ast::wildcard>{};
    inline constexpr auto digit = regex_interface<ast::digit>{};
    inline constexpr auto lower = regex_interface<ast::lower>{};
    inline constexpr auto upper = regex_interface<ast::upper>{};
    inline constexpr auto word = regex_interface<ast::word>{};
    inline constexpr auto hexa = regex_interface<ast::hexa>{};
    inline constexpr auto endl = regex_interface<ast::linebreak>{};

    template<char C>
    inline constexpr auto chr = regex_interface<ast::literal<C>>{};

    template<static_string String>
    inline constexpr auto str = to_regex(to_sequence<String>());

    template<char A, char B>
    inline constexpr auto range = regex_interface<ast::range<A, B>>{};

    template<std::size_t ID>
    inline constexpr auto group = regex_interface<ast::backref<ID>>{};

    template<static_string Name>
    inline constexpr auto group_named = regex_interface<ast::named_backref<symbol::name<Name>>>{};

    template<typename Node>
    constexpr auto negate(regex_interface<Node>) noexcept -> regex_interface<ast::negated<Node>> { return {}; }

    // Lookarounds
    template<typename... Nodes>
    constexpr auto ahead(regex_interface<Nodes>...) noexcept
    {
        return to_regex(pack<ast::positive_lookahead>(Nodes{} ...));
    }

    template<typename... Nodes>
    constexpr auto behind(regex_interface<Nodes>...) noexcept
    {
        return to_regex(pack<ast::positive_lookbehind>(Nodes{} ...));
    }

    template<typename Inner>
    constexpr auto negate(regex_interface<ast::positive_lookahead<Inner>>) noexcept
    {
        return to_regex(ast::negative_lookahead<Inner>{});
    }

    template<typename Inner>
    constexpr auto negate(regex_interface<ast::positive_lookbehind<Inner>>) noexcept
    {
        return to_regex(ast::negative_lookbehind<Inner>{});
    }

    // Builder that generates the AST from a regular expression
    template<static_string Pattern>
    inline constexpr auto regex = typename meta::regex<Pattern>::base_type{};
}
#endif //MREGEX_XPR_COMPONENTS_HPP