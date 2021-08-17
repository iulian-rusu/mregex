#ifndef META_CONCEPTS_HPP
#define META_CONCEPTS_HPP

#include <type_traits>

namespace meta
{
    template<typename From, typename To>
    concept convertible_to = std::is_convertible_v<From, To>;

    template<typename T>
    concept not_void = !std::is_same_v<std::remove_cvref_t<T>, void>;

    /**
     * Concept used to constrain the generic type accepted by matching/searching functions.
     */
    template<typename T, typename RRT = std::remove_reference_t<T>>
    concept string_like = requires(T s, std::size_t index)
    {
        { s[index] } -> convertible_to<char32_t>;
        { s.length() } -> convertible_to<std::size_t>;
        s.cbegin();
        s.cend();
        { RRT{s.cbegin(), s.cbegin() + index} } -> convertible_to<RRT>;
    };

    /**
     * Concept used to constrain a type used as a value producer for meta::generator.
     */
    template<typename T>
    concept producer = requires(T p)
    {
        { p() } -> not_void;
    };
}
#endif //META_CONCEPTS_HPP