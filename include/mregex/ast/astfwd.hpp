#ifndef MREGEX_AST_ASTFWD_HPP
#define MREGEX_AST_ASTFWD_HPP

#include <mregex/symbols/names.hpp>
#include <mregex/symbols/quantifiers.hpp>
#include <mregex/utility/enums.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct sequence;

    template<typename First, typename... Rest>
    struct alternation;

    template<std::size_t ID, typename Name, typename Inner>
    struct capture;

    template<std::size_t ID, static_string Name, typename Inner>
    using named_capture = capture<ID, symbol::name<Name>, Inner>;

    template<std::size_t ID, typename Inner>
    using unnamed_capture = capture<ID, symbol::unnamed, Inner>;

    // Repetition
    template<match_mode Mode, symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    struct basic_repetition;

    template<symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    using repetition = basic_repetition<match_mode::greedy, A, B, Inner>;

    template<symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    using lazy_repetition = basic_repetition<match_mode::lazy, A, B, Inner>;

    template<symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    using possessive_repetition = basic_repetition<match_mode::possessive, A, B, Inner>;

    // Fixed repetition
    template<match_mode Mode, std::size_t N, typename Inner>
    using basic_fixed_repetition = basic_repetition<Mode, symbol::quantifier_value<N>, symbol::quantifier_value<N>, Inner>;

    template<std::size_t N, typename Inner>
    using fixed_repetition = basic_fixed_repetition<match_mode::greedy, N, Inner>;

    template<std::size_t N, typename Inner>
    using lazy_fixed_repetition = basic_fixed_repetition<match_mode::lazy, N, Inner>;

    template<std::size_t N, typename Inner>
    using possessive_fixed_repetition = basic_fixed_repetition<match_mode::possessive, N, Inner>;

    template<typename Inner>
    using atomic = possessive_fixed_repetition<1, Inner>;

    // Kleene star
    template<match_mode Mode, typename Inner>
    using basic_star = basic_repetition<Mode, symbol::quantifier_value<0>, symbol::infinity, Inner>;

    template<typename Inner>
    using star = basic_star<match_mode::greedy, Inner>;

    template<typename Inner>
    using lazy_star = basic_star<match_mode::lazy, Inner>;

    template<typename Inner>
    using possessive_star = basic_star<match_mode::possessive, Inner>;

    // Plus
    template<match_mode Mode, typename Inner>
    using basic_plus = basic_repetition<Mode, symbol::quantifier_value<1>, symbol::infinity, Inner>;

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

    template<typename... Nodes>
    struct set;

    template<typename Inner>
    struct negated;

    // Terminals
    struct empty;
    struct beginning_of_line;
    struct beginning_of_input;
    struct end_of_line;
    struct end_of_input;
    struct word_boundary;
    struct wildcard;

    template<char C>
    struct literal;

    template<char A, char B>
    struct range;

    using lower = range<'a', 'z'>;
    using upper = range<'A', 'Z'>;
    using digit = range<'0', '9'>;

    using nothing = set<>;
    using word = set<lower, upper, digit, literal<'_'>>;
    using linebreak = set<literal<'\n'>, literal<'\r'>>;
    using whitespace = set<linebreak, literal<' '>, literal<'\t'>, literal<'\v'>, literal<'\f'>>;

    template<std::size_t ID>
    struct backref;

    template<static_string Name>
    struct named_backref;

    // Lookarounds
    template<lookaround_direction Direction, typename Inner>
    struct lookaround;

    template<typename Inner>
    using positive_lookahead = lookaround<lookaround_direction::ahead, Inner>;

    template<typename Inner>
    using negative_lookahead = negated<positive_lookahead<Inner>>;

    template<typename Inner>
    using positive_lookbehind = lookaround<lookaround_direction::behind, Inner>;

    template<typename Inner>
    using negative_lookbehind = negated<positive_lookbehind<Inner>>;
}
#endif //MREGEX_AST_ASTFWD_HPP