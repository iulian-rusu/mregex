#ifndef CX_META_HELPERS_H
#define CX_META_HELPERS_H

#include "../ast_fwd.h"

namespace cx
{
    template<typename First, typename Second>
    struct pair
    {
        using first = First;
        using second = Second;
    };

    /**
     * Metafunction returning the first type in a variadic type pack
     *
     * @tparam First    The type to be returned
     */
    template<typename First, typename ...>
    struct first
    {
        using type = First;
    };

    template<typename ... Elems>
    using first_t = typename first<Elems ...>::type;

    /**
     * Metafunction returning the last type in a variadic type pack
     *
     * @tparam First    The first type in the pack
     * @tparam Rest      The rest of the type pack
     */
    template<typename First, typename ... Rest>
    struct last
    {
        using type = typename last<Rest ...>::type;
    };

    template<typename First>
    struct last<First>
    {
        using type = First;
    };

    template<typename ... Elems>
    using last_t = typename last<Elems ...>::type;

    /***
    * Metafunction used to detect if a type is present inside a type pack
    */
    template<typename Test, typename ... Elems>
    struct is_any_of : std::bool_constant<(std::is_same_v<Test, Elems> || ... )> {};

    template<typename Test, typename ... Rest>
    constexpr bool is_any_of_v = is_any_of<Test, Rest ...>::value;

    /**
     * Traits to identify some AST nodes
     */
    template<typename T>
    constexpr bool is_terminal_v = std::is_base_of_v<terminal, T>;

    template<typename T>
    struct is_range : std::false_type {};

    template<auto A, auto B>
    struct is_range<range<A, B>> : std::true_type {};

    template<typename T>
    constexpr bool is_range_v = is_range<T>::value;

    template<typename T>
    struct is_alternation : std::false_type {};

    template<typename First, typename ... Rest>
    struct is_alternation<alternation<First, Rest ...>> : std::true_type {};

    template<typename T>
    constexpr bool is_alternation_v = is_alternation<T>::value;
}
#endif //CX_META_HELPERS_H
