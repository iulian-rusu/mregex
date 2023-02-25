#ifndef MREGEX_UTILITY_TYPE_TRAITS_HPP
#define MREGEX_UTILITY_TYPE_TRAITS_HPP

#include <type_traits>

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
     * Metafunction that removes references and CV-qualifiers from rvalues only.
     * Non-refernce types are also treated as rvalues.
     * References to lvalues are unchanged.
     */
    template<typename T>
    struct remove_rvalue_cvref
    {
        using type = std::remove_cvref_t<T>;
    };

    template<typename T>
    struct remove_rvalue_cvref<T &>
    {
        using type = T &;
    };

    template<typename T>
    using remove_rvalue_cvref_t = typename remove_rvalue_cvref<T>::type;
}
#endif //MREGEX_UTILITY_TYPE_TRAITS_HPP