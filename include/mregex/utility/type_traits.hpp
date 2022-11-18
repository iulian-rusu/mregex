#ifndef MREGEX_UTILITY_TYPE_TRAITS_HPP
#define MREGEX_UTILITY_TYPE_TRAITS_HPP

#include <type_traits>
#include <mregex/symbols.hpp>

namespace meta
{
    /**
     * Metafunction used to detect if a type is present inside a type pack.
     */
    template<typename Test, typename... Elems>
    inline constexpr bool is_any_of = (std::is_same_v<Test, Elems> || ...);

    /**
     * Type trait that checks if T is an rvalue reference to an object that is non-trivially destructible.
     * This usually indicates a temporary object which needs to free memory upon destruction.
     * Although this does not guarantee that T has dynamically allocated memory, it is a good heuristic.
     */
    template<typename T>
    inline constexpr bool is_expiring_memory_owner = std::is_rvalue_reference_v<T> &&
                                                     !std::is_trivially_destructible_v<std::remove_reference_t<T>>;

    /**
     * Metafunction that deduces the type required to perfectly forward the result of a function.
     * Return types that are lvalue references are forwarded to a reference.
     * Other value categories are forwarded (moved or copy elided) to a value type.
     */
    template<typename T>
    struct forward_result : std::type_identity<T> {};

    template<typename T>
    struct forward_result<T &&> : std::type_identity<T> {};

    template<typename T>
    using forward_result_t = typename forward_result<T>::type;

    /**
     * Metafunction that converts a pack of characters to a symbolic name type.
     *
     * @note The sizeof... (Chars) is technically redundant but GCC 11.1 cannot deduce the size.
     */
    template<char... Chars>
    using make_name = symbol::name<static_string<sizeof... (Chars)>{{Chars ..., '\0'}}>;
}
#endif //MREGEX_UTILITY_TYPE_TRAITS_HPP