#ifndef CX_ASTFWD_H
#define CX_ASTFWD_H

namespace cx
{
    /**
     * Forward declaration of basic AST node types for type resolution.
     */

    template<typename, typename ...>
    struct sequence;

    template<typename, typename ...>
    struct alternation;

    template<typename, typename ...>
    struct disjunction;

    template<typename>
    struct star;

    template<std::size_t, typename>
    struct repeated;

    struct terminal;

    struct epsilon;

    struct nothing;

    struct beginning;

    struct ending;

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

    using word = alternation<alpha, digit, character<'_'>>;

    using hexa = alternation<digit, range<'a', 'f'>, range<'A', 'F'>>;

    template<std::size_t>
    struct backref;

    template<std::size_t, typename>
    struct atomic;

    template<std::size_t, typename>
    struct capturing;

    template<typename>
    struct negated;
}
#endif //CX_ASTFWD_H