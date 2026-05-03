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

        template<typename T>
        inline constexpr bool is_capture_storage = false;

        template<typename... Elems>
        inline constexpr bool is_capture_storage<std::tuple<Elems ...>> = (captured_content<Elems> && ...);
    }

    /**
     * Concept used to constrain a type that stores regex captures.
     */
    template<typename CaptureStorage>
    concept capture_storage = detail::is_capture_storage<std::remove_cvref_t<CaptureStorage>>;

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

    template<capture_storage CaptureStorage, static_string Name>
    using named_capture_type_for = rename_capture_t<std::tuple_element_t<0, std::remove_reference_t<CaptureStorage>>, symbol::name<Name>>;

    /**
     * Helpers for accessing capturing groups.
     */

    template<std::size_t ID, capture_storage CaptureStorage>
    constexpr decltype(auto) get_group(CaptureStorage &&captures) noexcept
    {
        constexpr std::size_t group_count = std::tuple_size_v<std::remove_reference_t<CaptureStorage>>;

        static_assert(ID < group_count, "capturing group does not exist");
        return std::get<ID>(std::forward<CaptureStorage>(captures));
    }

    template<static_string Name, capture_storage CaptureStorage>
    constexpr decltype(auto) get_group(CaptureStorage &&captures) noexcept
    {
        using capture_type = named_capture_type_for<CaptureStorage, Name>;
        return std::get<capture_type>(std::forward<CaptureStorage>(captures));
    }
}
#endif //MREGEX_REGEX_CAPTURE_STORAGE_HPP