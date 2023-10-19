#ifndef MREGEX_UTILITY_CONCEPTS_HPP
#define MREGEX_UTILITY_CONCEPTS_HPP

#include <concepts>

namespace meta
{
    /**
     * Concept used to constrain the generic type accepted by matching/searching functions.
     */
    template<typename Range>
    concept char_range = std::ranges::forward_range<Range> && requires (Range range)
    {
        { *std::begin(range) } -> std::convertible_to<char32_t>;
    };

    /**
     * Concept used to constrain a functor type that can be called to generate
     * values convertible to bool.
     */
    template<typename Generator>
    concept bool_testable_generator = requires (Generator generator)
    {
        static_cast<bool>(generator());
    };
}
#endif //MREGEX_UTILITY_CONCEPTS_HPP