#ifndef MREGEX_REGEX_MATCH_CONTEXT_HPP
#define MREGEX_REGEX_MATCH_CONTEXT_HPP

#include <mregex/utility/tuple.hpp>
#include <mregex/regex_capture_storage.hpp>

namespace meta
{
    /**
     * Data structure that holds static information about the regex as well as
     * dynamic information required for matching.
     *
     * @tparam Regex    The regex type used for matching
     * @tparam Iter     The forward iterator type used to access the input
     */
    template<typename Regex, std::forward_iterator Iter>
    struct regex_match_context
    {
        using regex_type = Regex;
        using iterator = Iter;
        using ast_type = regex_ast_t<regex_type>;
        using flags = regex_flags<regex_type>;

        regex_capture_view_storage<regex_type, iterator> captures{};

        constexpr void clear() noexcept
        {
            iterate_tuple(captures, [](auto &capture) noexcept {
                capture.clear();
            });
        }
    };
}
#endif //MREGEX_REGEX_MATCH_CONTEXT_HPP