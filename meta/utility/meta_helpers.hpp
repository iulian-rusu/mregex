#ifndef META_META_HELPERS_HPP
#define META_META_HELPERS_HPP

#include "../ast/astfwd.hpp"

namespace meta
{
    template<typename First, typename Second>
    struct pair
    {
        using first = First;
        using second = Second;
    };

    /**
     * Metafunction returning the first type in a variadic type pack.
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
     * Metafunction returning the last type in a variadic type pack.
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
    * Metafunction used to detect if a type is present inside a type pack.
    */
    template<typename Test, typename ... Elems>
    struct is_any_of : std::bool_constant<(std::is_same_v<Test, Elems> || ... )> {};

    template<typename Test, typename ... Rest>
    constexpr bool is_any_of_v = is_any_of<Test, Rest ...>::value;
}
#endif //META_META_HELPERS_HPP