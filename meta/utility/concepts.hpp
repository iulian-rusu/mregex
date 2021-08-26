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
    concept string_like = requires(T s, std::size_t index)
    {
        { s[index] } -> std::convertible_to<char32_t>;
        { s.length() } -> std::same_as<std::size_t>;
        { s.begin() } -> std::forward_iterator;
        { s.end() } -> std::forward_iterator;
    };

    template<typename T>
    concept capture_like = requires(T c)
    {
        { c.length() } -> std::convertible_to<std::size_t>;
        { c.begin() } -> std::forward_iterator;
        { c.end() } -> std::forward_iterator;
        { c.get() } -> string_like;
    };

    template<typename T>
    concept capture_storage = requires(T s)
    {
        { std::get<0>(s) } -> capture_like;
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