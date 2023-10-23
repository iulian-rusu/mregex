#ifndef MREGEX_UTILITY_TYPE_TRAITS_HPP
#define MREGEX_UTILITY_TYPE_TRAITS_HPP

#include <type_traits>

namespace meta
{
    /**
     * Type trait that checks if T is an rvalue reference to an object that is non-trivially destructible.
     * This usually indicates a temporary object which needs to free memory upon destruction.
     * Although this does not guarantee that T has dynamically allocated memory, it is a good heuristic.
     *
     * @tparam T    The (possibly CV-qualified) type
     */
    template<typename T>
    inline constexpr bool is_expiring_memory_owner = std::is_rvalue_reference_v<T> &&
                                                     !std::is_trivially_destructible_v<std::remove_reference_t<T>>;
}
#endif //MREGEX_UTILITY_TYPE_TRAITS_HPP