#ifndef MREGEX_SYMBOLS_QUANTIFIERS_HPP
#define MREGEX_SYMBOLS_QUANTIFIERS_HPP

#include <type_traits>

namespace meta::symbol
{
    /**
     * Symbols for parsing and working with quantifiers.
     */

    struct quantifier_begin {};

    template<typename Action>
    struct quantifier_mod {};

    struct unquantifiable {};
    struct infinity {};

    template<std::size_t N>
    struct quantifier_value {};

    /**
    * Type trait used to check if a type is a symbolic quantifier.
    */
    template<typename Symbol>
    inline constexpr bool is_quantifier = false;

    template<std::size_t N>
    inline constexpr bool is_quantifier<quantifier_value<N>> = true;

    template<>
    inline constexpr bool is_quantifier<infinity> = true;

    template<typename Symbol>
    concept quantifier = is_quantifier<Symbol>;

    /**
     * Predicate that checks if a symbolic quantifier is equivalent to infinity.
     */
    template<quantifier>
    inline constexpr bool is_infinity = false;

    template<>
    inline constexpr bool is_infinity<infinity> = true;

    template<typename Symbol>
    concept finite_quantifier = quantifier<Symbol> && !is_infinity<Symbol>;

    /**
     * Predicate that checks if a symbolic quantifier is equivalent to 0.
     */
    template<quantifier Symbol>
    inline constexpr bool is_zero = false;

    template<>
    inline constexpr bool is_zero<quantifier_value<0>> = true;

    /**
     * Predicate that checks if a pair of quantifiers for a valid range (interval).
     */
    template<quantifier A, quantifier B>
    inline constexpr bool is_valid_range = false;

    template<std::size_t A, std::size_t B>
    inline constexpr bool is_valid_range<quantifier_value<A>, quantifier_value<B>> = A <= B;

    template<std::size_t N>
    inline constexpr bool is_valid_range<quantifier_value<N>, infinity> = true;

    /**
     * Metafunction that projects a finite symbolic quantifier to its numerical value.
     */
    template<finite_quantifier Symbol>
    inline constexpr std::size_t get_value = {};

    template<std::size_t N>
    inline constexpr std::size_t get_value<quantifier_value<N>> = N;

    /**
     * Predicate that checks if a symbolic quantifier is equal to some integral value.
     */
    template<quantifier Symbol>
    inline constexpr auto equals = [](std::size_t) noexcept { return false; };

    template<std::size_t N>
    inline constexpr auto equals<quantifier_value<N>> = [](std::size_t value) noexcept { return N == value; };

    /**
     * Metafunction used to decrement symbolic quantifiers.
     */
    template<quantifier Symbol>
    struct decrement;

    template<std::size_t N>
    requires (N > 0)
    struct decrement<quantifier_value<N>>
    {
        using type = quantifier_value<N - 1>;
    };

    template<>
    struct decrement<infinity>
    {
        using type = infinity;
    };

    template<quantifier Symbol>
    using decrement_t = typename decrement<Symbol>::type;

    /**
     * Metafunction that handles subtraction of symbolic quantifiers.
     */
    template<quantifier A, quantifier B>
    struct subtract;

    template<std::size_t A, std::size_t B>
    requires (A >= B)
    struct subtract<quantifier_value<A>, quantifier_value<B>>
    {
        using type = quantifier_value<A - B>;
    };

    template<std::size_t N>
    struct subtract<infinity, quantifier_value<N>>
    {
        using type = infinity;
    };

    template<quantifier A, quantifier B>
    using subtract_t = typename subtract<A, B>::type;

    /**
     * Metacontainer for the upper and lower bound of a range.
     */
    template<symbol::finite_quantifier A, symbol::quantifier B>
    struct quantifier_range {};
}
#endif //MREGEX_SYMBOLS_QUANTIFIERS_HPP