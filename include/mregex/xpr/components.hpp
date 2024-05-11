#ifndef MREGEX_XPR_COMPONENTS_HPP
#define MREGEX_XPR_COMPONENTS_HPP

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
    constexpr auto concat(regex_adapter<Nodes>...) noexcept
    {
        return to_regex(flat_wrap_sequence<ast::sequence>(Nodes{} ...));
    }

    // Alternations
    template<typename... Nodes>
    constexpr auto either(regex_adapter<Nodes>...) noexcept
    {
        return to_regex(flat_wrap_sequence<ast::alternation>(Nodes{} ...));
    }

    // Atomics
    template<typename... Nodes>
    constexpr auto atomic(regex_adapter<Nodes>...) noexcept
    {
        return to_regex(pack_sequence<ast::atomic>(Nodes{} ...));
    }

    // Captures
    template<std::size_t ID, static_string Name, typename... Nodes>
    constexpr auto capture(regex_adapter<Nodes>...) noexcept
    {
        using wrapper_provider = capture_provider<ID, symbol::name<Name>>;
        return to_regex(pack_sequence<wrapper_provider::template type>(Nodes{} ...));
    }

    template<std::size_t ID, typename... Nodes>
    constexpr auto capture(regex_adapter<Nodes>...) noexcept
    {
        using wrapper_provider = capture_provider<ID, symbol::unnamed>;
        return to_regex(pack_sequence<wrapper_provider::template type>(Nodes{} ...));
    }

    // Repetition
    template<std::size_t A, std::size_t B, match_mode Mode, typename... Nodes>
    constexpr auto between(regex_adapter<Nodes>...) noexcept
    {
        using wrapper_provider = repetition_provider<Mode, symbol::quantifier_value<A>, symbol::quantifier_value<B>>;
        return to_regex(pack_sequence<wrapper_provider::template type>(Nodes{} ...));
    }

    template<std::size_t A, std::size_t B, typename... Nodes>
    constexpr auto between(regex_adapter<Nodes>... expressions) noexcept
    {
        return between<A, B, match_mode::greedy>(expressions ...);
    }

    template<std::size_t N, match_mode Mode, typename... Nodes>
    constexpr auto at_least(regex_adapter<Nodes>...) noexcept
    {
        using wrapper_provider = repetition_provider<Mode, symbol::quantifier_value<N>, symbol::infinity>;
        return to_regex(pack_sequence<wrapper_provider::template type>(Nodes{} ...));
    }

    template<std::size_t N, typename... Nodes>
    constexpr auto at_least(regex_adapter<Nodes>... expressions) noexcept
    {
        return at_least<N, match_mode::greedy>(expressions ...);
    }

    // Fixed repetition
    template<std::size_t N, match_mode Mode, typename... Nodes>
    constexpr auto exactly(regex_adapter<Nodes>... expressions) noexcept
    {
        return between<N, N, Mode>(expressions ...);
    }

    template<std::size_t N, typename... Nodes>
    constexpr auto exactly(regex_adapter<Nodes>... expressions) noexcept
    {
        return between<N, N>(expressions ...);
    }

    // Kleene star
    template<match_mode Mode, typename... Nodes>
    constexpr auto zero_or_more(regex_adapter<Nodes>... expressions) noexcept
    {
        return at_least<0, Mode>(expressions ...);
    }

    template<typename... Nodes>
    constexpr auto zero_or_more(regex_adapter<Nodes>... expressions) noexcept
    {
        return at_least<0>(expressions ...);
    }

    // Plus
    template<match_mode Mode, typename... Nodes>
    constexpr auto one_or_more(regex_adapter<Nodes>... expressions) noexcept
    {
        return at_least<1, Mode>(expressions ...);
    }

    template<typename... Nodes>
    constexpr auto one_or_more(regex_adapter<Nodes>... expressions) noexcept
    {
        return at_least<1>(expressions ...);
    }

    // Optional
    template<match_mode Mode, typename... Nodes>
    constexpr auto maybe(regex_adapter<Nodes>... expressions) noexcept
    {
        return between<0, 1, Mode>(expressions ...);
    }

    template<typename... Nodes>
    constexpr auto maybe(regex_adapter<Nodes>... expressions) noexcept
    {
        return between<0, 1>(expressions ...);
    }

    // Terminals
    inline constexpr auto empty = regex_adapter<ast::empty>{};
    inline constexpr auto nothing = regex_adapter<ast::nothing>{};
    inline constexpr auto begin_line = regex_adapter<ast::beginning_of_line>{};
    inline constexpr auto end_line = regex_adapter<ast::end_of_line>{};
    inline constexpr auto begin_input = regex_adapter<ast::beginning_of_input>{};
    inline constexpr auto end_input = regex_adapter<ast::end_of_input>{};
    inline constexpr auto word_boundary = regex_adapter<ast::word_boundary>{};
    inline constexpr auto whitespace = regex_adapter<ast::whitespace>{};
    inline constexpr auto wildcard = regex_adapter<ast::wildcard>{};
    inline constexpr auto lower = regex_adapter<ast::lower>{};
    inline constexpr auto upper = regex_adapter<ast::upper>{};
    inline constexpr auto digit = regex_adapter<ast::digit>{};
    inline constexpr auto word = regex_adapter<ast::word>{};
    inline constexpr auto linebreak = regex_adapter<ast::linebreak>{};

    template<char C>
    inline constexpr auto chr = regex_adapter<ast::literal<C>>{};

    template<static_string String>
    inline constexpr auto str = to_regex(to_sequence<String>());

    template<char A, char B>
    inline constexpr auto range = regex_adapter<ast::range<A, B>>{};

    template<std::size_t ID>
    inline constexpr auto group = regex_adapter<ast::backref<ID>>{};

    template<static_string Name>
    inline constexpr auto group_named = regex_adapter<ast::named_backref<Name>>{};

    template<typename Node>
    constexpr auto negate(regex_adapter<Node>) noexcept -> regex_adapter<ast::negated<Node>> { return {}; }

    // Lookarounds
    template<typename... Nodes>
    constexpr auto ahead(regex_adapter<Nodes>...) noexcept
    {
        return to_regex(pack_sequence<ast::positive_lookahead>(Nodes{} ...));
    }

    template<typename... Nodes>
    constexpr auto behind(regex_adapter<Nodes>...) noexcept
    {
        return to_regex(pack_sequence<ast::positive_lookbehind>(Nodes{} ...));
    }

    template<static_string Pattern>
    inline constexpr auto regex = meta::regex<Pattern>{};
}
#endif //MREGEX_XPR_COMPONENTS_HPP