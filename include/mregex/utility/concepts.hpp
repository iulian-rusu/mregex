#ifndef MREGEX_UTILITY_CONCEPTS_HPP
#define MREGEX_UTILITY_CONCEPTS_HPP

#include <concepts>
#include <tuple>
#include <type_traits>

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
     * Concept used to constrain a type that saves the content captured by regex groups.
     */
    template<typename Capture>
    concept captured_content = std::ranges::forward_range<Capture> && requires (Capture capture)
    {
        { capture.content() } -> char_range;
        { capture.length() } -> std::convertible_to<std::size_t>;
        static_cast<bool>(capture);
    };

    /**
     * Concept used to constrain a type that stores multiple regex captures.
     */
    template<typename CaptureStorage>
    concept capture_storage = requires (CaptureStorage captures)
    {
        { std::tuple_size_v<std::remove_reference_t<CaptureStorage>> } -> std::convertible_to<std::size_t>;
        { std::get<0>(captures) } -> captured_content;
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