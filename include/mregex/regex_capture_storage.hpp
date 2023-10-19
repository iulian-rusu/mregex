#ifndef MREGEX_REGEX_CAPTURE_STORAGE_HPP
#define MREGEX_REGEX_CAPTURE_STORAGE_HPP

#include <mregex/regex_capture.hpp>
#include <mregex/regex_traits.hpp>

namespace meta
{
    namespace detail
    {
        template<std::forward_iterator Iter, typename... Names>
        constexpr auto make_regex_capture_view_storage(type_sequence<Names ...>) noexcept
        {
            return std::tuple<regex_capture_view<Iter, Names> ...>{};
        }

        template<typename... Names>
        constexpr auto make_regex_capture_storage(type_sequence<Names ...>) noexcept
        {
            return std::tuple<regex_capture<Names> ...>{};
        }
    }

    /**
     * Concept used to constrain a type that stores regex captures.
     */
    template<typename CaptureStorage>
    concept capture_storage = requires(CaptureStorage captures)
    {
        { std::tuple_size_v<std::remove_reference_t<CaptureStorage>> } -> std::convertible_to<std::size_t>;
        { std::get<0>(captures) } -> captured_content;
    };

    /**
     * Storage type that holds views into regex captures.
     *
     * @tparam Regex    The regex type used for matching
     * @tparam Iter     The forward iterator type used to view the captured content
     */
    template<typename Regex, std::forward_iterator Iter>
    using regex_capture_view_storage = decltype(detail::make_regex_capture_view_storage<Iter>(regex_capture_name_spec_t<Regex>{}));

    /**
     * Storage type that holds memory-owning regex captures.
     *
     * @tparam Regex    The regex type used for matching
     */
    template<typename Regex>
    using regex_capture_storage = decltype(detail::make_regex_capture_storage(regex_capture_name_spec_t<Regex>{}));
}
#endif //MREGEX_REGEX_CAPTURE_STORAGE_HPP
