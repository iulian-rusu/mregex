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
        { std::remove_reference_t<T>{s.begin(), s.end() + index} } -> std::convertible_to<std::string_view>;
    };

    /**
     * Concept used to constrain a type used as a value iterable_producer for meta::generator.
     * The return type must be explicitly convertible to bool to signal iteration ending.
     */
    template<typename T>
    concept iterable_producer = requires(T p)
    {
        static_cast<bool>(p());
    };
}
#endif //META_CONCEPTS_HPP