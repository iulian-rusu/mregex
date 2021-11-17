#ifndef MREGEX_MREGEX_HELPERS_HPP
#define MREGEX_MREGEX_HELPERS_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta
{
    template<typename... >
    struct first;

    template<typename First, typename... Rest>
    struct first<First, Rest ...>
    {
        using type = First;
    };

    template<typename... Elems>
    using first_t = typename first<Elems ...>::type;

    template<typename... >
    struct last;

    template<typename First, typename... Rest>
    struct last<First, Rest ...>
    {
        using type = typename last<Rest ...>::type;
    };

    template<typename First>
    struct last<First>
    {
        using type = First;
    };

    template<typename... Elems>
    using last_t = typename last<Elems ...>::type;

    /**
     * Metafunction used to detect if a type is present inside a type pack.
     */
    template<typename Test, typename... Elems>
    struct is_any_of : std::bool_constant<(std::is_same_v<Test, Elems> || ...)> {};

    template<typename Test, typename... Rest>
    inline constexpr bool is_any_of_v = is_any_of<Test, Rest ...>::value;

    /**
     * Type trait that checks if T is an rvalue reference to a memory-owning object.
     * That is, a temporary object that holds allocated memory.
     */
    template<typename T>
    struct is_memory_owning_rvalue
    {
        using raw_type = std::remove_reference_t<T>;

        static constexpr bool value = !std::is_trivially_destructible_v<raw_type> && std::is_rvalue_reference_v<T &&>;
    };

    template<typename T>
    inline constexpr bool is_memory_owning_rvalue_v = is_memory_owning_rvalue<T>::value;
}
#endif //MREGEX_MREGEX_HELPERS_HPP