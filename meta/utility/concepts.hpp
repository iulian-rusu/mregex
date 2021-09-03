#ifndef META_CONCEPTS_HPP
#define META_CONCEPTS_HPP

#include <type_traits>
#include <concepts>
#include <tuple>

namespace meta
{
    /**
     * Concept used to constrain the generic type accepted by matching/searching functions.
     */
    template<typename T>
    concept string_range = std::ranges::forward_range<T> && requires(T s)
    {
        { *s.begin() } -> std::convertible_to<char32_t>;
        { s.length() } -> std::convertible_to<std::size_t>;
    };

    /**
     * Concept used to constrain a type that saves capturing group matches.
     */
    template<typename T>
    concept capture_range = std::ranges::forward_range<T> && requires(T c)
    {
        { c.length() } -> std::convertible_to<std::size_t>;
        { c.content() } -> string_range;
    };

    /**
     * Concept used to constrain a type that stores multiple regex captures.
     */
    template<typename T>
    concept capture_storage = requires(T s)
    {
        { std::get<0>(s) } -> capture_range;
    };

    /**
     * Concept used to constrain a type that generates values that can be explicitly
     * converted to booleans.
     */
    template<typename T>
    concept bool_testable_generator = requires(T p)
    {
        static_cast<bool>(p());
    };
}
#endif //META_CONCEPTS_HPP