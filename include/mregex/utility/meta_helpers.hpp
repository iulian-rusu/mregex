#ifndef MREGEX_MREGEX_HELPERS_HPP
#define MREGEX_MREGEX_HELPERS_HPP

#include <type_traits>

namespace meta
{
    /**
     * Metafunction used to detect if a type is present inside a type pack.
     */
    template<typename Test, typename... Elems>
    struct is_any_of : std::bool_constant<(std::is_same_v<Test, Elems> || ...)> {};

    template<typename Test, typename... Elems>
    inline constexpr bool is_any_of_v = is_any_of<Test, Elems ...>::value;

    /**
     * Type trait that checks if T is an rvalue reference to an object that is non-trivially destructible.
     * This usually indicates a temporary object which needs to free memory upon destruction.
     * Although this does not guarantee that T has dynamically allocated memory, it is a good heuristic.
     */
    template<typename T>
    struct is_expiring_memory_owner
    {
        using raw_type = std::remove_reference_t<T>;

        static constexpr bool value = !std::is_trivially_destructible_v<raw_type> && std::is_rvalue_reference_v<T>;
    };

    template<typename T>
    inline constexpr bool is_expiring_memory_owner_v = is_expiring_memory_owner<T>::value;
}
#endif //MREGEX_MREGEX_HELPERS_HPP