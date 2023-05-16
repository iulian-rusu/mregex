#ifndef MREGEX_REGEX_CONTEXT_HPP
#define MREGEX_REGEX_CONTEXT_HPP

#include <mregex/utility/tuple.hpp>
#include <mregex/regex_traits.hpp>

namespace meta
{
    /**
     * Data structure that holds static information about the regex as well as
     * match-time information like captures.
     *
     * @tparam Regex    The regex type used for matching
     * @tparam Iter     The forward iterator type used to acces the input
     */
    template<typename Regex, std::forward_iterator Iter>
    struct regex_context
    {
        using regex_type = Regex;
        using iterator_type = Iter;
        using ast_type = regex_ast_t<regex_type>;
        using flags = regex_flags<regex_type>;
        using capture_view_storage_type = regex_capture_view_storage_t<regex_type, iterator_type>;

        capture_view_storage_type captures{};

        constexpr void clear() noexcept
        {
            iterate_tuple(captures, [](auto &capture) noexcept {
                capture.clear();
            });
        }
    };
}
#endif //MREGEX_REGEX_CONTEXT_HPP