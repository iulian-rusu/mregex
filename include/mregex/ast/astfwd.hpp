#ifndef MREGEX_ASTFWD_HPP
#define MREGEX_ASTFWD_HPP

#include <mregex/symbol.hpp>

namespace meta::ast
{
    template<typename, typename...>
    struct sequence;

    template<typename, typename...>
    struct alternation;

    template<typename, typename...>
    struct disjunction;

    template<typename>
    struct star;

    template<typename, typename, typename>
    struct repetition;

    struct terminal;

    struct epsilon;

    struct nothing;

    template<typename, typename...>
    struct set;

    struct beginning;

    struct ending;

    template<auto>
    struct literal;

    struct whitespace;

    struct wildcard;

    template<auto, auto>
    struct range;

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = repetition<symbol::quantifier_value<1>, symbol::quantifier_inf, T>;

    using digit = range<'0', '9'>;

    using lower = range<'a', 'z'>;

    using upper = range<'A', 'Z'>;

    using alpha = range<'A', 'z'>;

    using word = set<alpha, digit, literal<'_'>>;

    using hexa = set<digit, range<'a', 'f'>, range<'A', 'F'>>;

    using linebreak = set<literal<'\n'>, literal<'\r'>>;

    template<std::size_t>
    struct backref;

    template<typename>
    struct negated;

    template<std::size_t, typename>
    struct capturing;

    template<typename>
    struct positive_lookahead;

    template<typename>
    struct negative_lookahead;
}
#endif //MREGEX_ASTFWD_HPP