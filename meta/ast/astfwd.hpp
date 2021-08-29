#ifndef META_ASTFWD_HPP
#define META_ASTFWD_HPP

#include "../symbol.hpp"

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

    struct beginline;

    struct endline;

    struct endsequence;

    template<auto>
    struct character;

    struct whitespace;

    struct wildcard;

    template<auto, auto>
    struct range;

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = sequence<T, star<T>>;

    using digit = range<'0', '9'>;

    using lower = range<'a', 'z'>;

    using upper = range<'A', 'Z'>;

    using alpha = range<'A', 'z'>;

    using word = set<alpha, digit, ast::character<'_'>>;

    using hexa = set<digit, range<'a', 'f'>, range<'A', 'F'>>;

    template<std::size_t>
    struct backref;

    template<typename>
    struct atomic;

    template<std::size_t, typename>
    struct capturing;

    template<typename>
    struct negated;
}
#endif //META_ASTFWD_HPP