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
    template<typename R>
    concept char_range = std::ranges::forward_range<R> && requires(R range)
    {
        { *range.begin() } -> std::convertible_to<char32_t>;
        { range.length() } -> std::convertible_to<std::size_t>;
    };

    /**
     * Concept used to constrain a type that saves capturing group matches.
     */
    template<typename R>
    concept capture_range = std::ranges::forward_range<R> && requires(R range)
    {
        { range.content() } -> char_range;
        { range.length() } -> std::convertible_to<std::size_t>;
    };

    /**
     * Concept used to constrain a type that stores multiple regex captures.
     */
    template<typename S>
    concept capture_storage = requires(S storage)
    {
        { std::get<0>(storage) } -> capture_range;
    };

    /**
     * Concept used to constrain a functor type that can be invoked to generate
     * values explicitly convertible to bool.
     */
    template<typename G>
    concept bool_testable_generator = requires(G gen)
    {
        static_cast<bool>(gen());
    };
}
#endif //META_CONCEPTS_HPP