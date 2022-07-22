#ifndef MREGEX_ASTFWD_HPP
#define MREGEX_ASTFWD_HPP

#include <mregex/symbols.hpp>

namespace meta::ast
{
    template<typename, typename...>
    struct sequence;

    template<typename, typename...>
    struct alternation;

    template<std::size_t, typename, typename>
    struct capture;

    // Repetition
    template<match_mode, typename, typename, typename>
    struct basic_repetition;

    template<typename A, typename B, typename Inner>
    using repetition = basic_repetition<match_mode::greedy, A, B, Inner>;

    template<typename A, typename B, typename Inner>
    using lazy_repetition = basic_repetition<match_mode::lazy, A, B, Inner>;

    template<typename A, typename B, typename Inner>
    using possessive_repetition = basic_repetition<match_mode::possessive, A, B, Inner>;

    // Exact repetition
    template<match_mode Mode, std::size_t N, typename Inner>
    using basic_fixed_repetition = basic_repetition<Mode, symbol::quantifier_value<N>, symbol::quantifier_value<N>, Inner>;

    template<std::size_t N, typename Inner>
    using fixed_repetition = basic_fixed_repetition<match_mode::greedy, N, Inner>;

    template<std::size_t N, typename Inner>
    using lazy_fixed_repetition = basic_fixed_repetition<match_mode::lazy, N, Inner>;

    template<std::size_t N, typename Inner>
    using possessive_fixed_repetition = basic_fixed_repetition<match_mode::possessive, N, Inner>;

    // Kleene star
    template<match_mode Mode, typename Inner>
    using basic_star = basic_repetition<Mode, symbol::quantifier_value<0>, symbol::quantifier_inf, Inner>;

    template<typename Inner>
    using star = basic_star<match_mode::greedy, Inner>;

    template<typename Inner>
    using lazy_star = basic_star<match_mode::lazy, Inner>;

    template<typename Inner>
    using possessive_star = basic_star<match_mode::possessive, Inner>;

    // Plus
    template<match_mode Mode, typename Inner>
    using basic_plus = basic_repetition<Mode, symbol::quantifier_value<1>, symbol::quantifier_inf, Inner>;

    template<typename Inner>
    using plus = basic_plus<match_mode::greedy, Inner>;

    template<typename Inner>
    using lazy_plus = basic_plus<match_mode::lazy, Inner>;

    template<typename Inner>
    using possessive_plus = basic_plus<match_mode::possessive, Inner>;

    // Optional
    template<match_mode Mode, typename Inner>
    using basic_optional = basic_repetition<Mode, symbol::quantifier_value<0>, symbol::quantifier_value<1>, Inner>;

    template<typename Inner>
    using optional = basic_optional<match_mode::greedy, Inner>;

    template<typename Inner>
    using lazy_optional = basic_optional<match_mode::lazy, Inner>;

    template<typename Inner>
    using possessive_optional = basic_optional<match_mode::possessive, Inner>;

    // Terminals
    struct terminal;

    struct empty;

    struct nothing;

    template<typename, typename...>
    struct set;

    struct beginning;

    struct ending;

    template<char>
    struct literal;

    struct whitespace;

    struct wildcard;

    template<char, char>
    struct range;

    using digit = range<'0', '9'>;

    using lower = range<'a', 'z'>;

    using upper = range<'A', 'Z'>;

    using alpha = set<lower, upper>;

    using word = set<lower, upper, digit, literal<'_'>>;

    using hexa = set<digit, range<'a', 'f'>, range<'A', 'F'>>;

    using linebreak = set<literal<'\n'>, literal<'\r'>>;

    template<std::size_t>
    struct backref;

    template<typename>
    struct named_backref;

    template<typename>
    struct negated;

    // Lookarounds
    template<typename>
    struct positive_lookahead;

    template<typename>
    struct negative_lookahead;

    template<typename>
    struct positive_lookbehind;

    template<typename>
    struct negative_lookbehind;
}
#endif //MREGEX_ASTFWD_HPP