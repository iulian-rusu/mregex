#ifndef MREGEX_CONCEPTS_HPP
#define MREGEX_CONCEPTS_HPP

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
        { *std::ranges::begin(range) } -> std::convertible_to<char32_t>;
    };

    /**
     * Concept used to constrain a type that saves the content captured by regex groups.
     */
    template<typename R>
    concept captured_content = std::ranges::forward_range<R> && requires(R range)
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
        { std::get<0>(storage) } -> captured_content;
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
#endif //MREGEX_CONCEPTS_HPP