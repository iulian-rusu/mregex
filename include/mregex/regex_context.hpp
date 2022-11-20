#ifndef MREGEX_REGEX_CONTEXT_HPP
#define MREGEX_REGEX_CONTEXT_HPP

#include <mregex/utility/tuple.hpp>
#include <mregex/regex_flags.hpp>
#include <mregex/regex_result.hpp>
#include <mregex/regex_traits.hpp>

namespace meta
{
    /**
     * Data structure that holds static information about the regex as well as
     * match-time information like capturing group contents.
     *
     * @tparam Regex    The regex type used for matching
     * @tparam Iter     The forward iterator type used to acces the input sequence
     */
    template<typename Regex, std::forward_iterator Iter>
    struct regex_context
    {
        using iterator_type = Iter;
        using ast_type = regex_ast_t<Regex>;
        using flags = regex_flags<Regex>;
        using capture_view_storage_type = regex_capture_view_storage_t<Regex, Iter>;

        capture_view_storage_type captures{};

        constexpr void clear() noexcept
        {
            iterate_tuple(captures, [](auto &capture) {
                capture.clear();
            });
        }
    };

    template<typename Context>
    using context_flags = typename Context::flags;
}
#endif //MREGEX_REGEX_CONTEXT_HPP