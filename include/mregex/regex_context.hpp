#ifndef MREGEX_REGEX_CONTEXT_HPP
#define MREGEX_REGEX_CONTEXT_HPP

#include <mregex/utility/tuple_helpers.hpp>
#include <mregex/regex_flags.hpp>
#include <mregex/regex_capture.hpp>

namespace meta
{
    /**
     * Data structure that holds static information about the regex as well as
     * match-time information like iterator types and captured contents.
     */
    template<std::forward_iterator Iter, typename AST, typename... Flags>
    struct regex_context
    {
        static_assert((is_flag_v<Flags> && ...), "invalid flag");

        struct flags
        {
            static constexpr bool ignore_case = is_flag_enabled_v<flag::icase, Flags ...>;
            static constexpr bool dotall = is_flag_enabled_v<flag::dotall, Flags ...>;
            static constexpr bool multiline = is_flag_enabled_v<flag::multiline, Flags ...>;
            static constexpr bool ungreedy = is_flag_enabled_v<flag::ungreedy, Flags ...>;
            static constexpr bool unroll = is_flag_enabled_v<flag::unroll, Flags ...>;
        };

        using ast_type = AST;
        using iterator_type = Iter;

        regex_capture_view_storage<AST::capture_count, Iter> captures{};

        constexpr void clear() noexcept
        {
            iterate_tuple(captures, [](auto &capture) {
                capture.clear();
            });
        }
    };

    template<typename Context>
    using flags_of = typename Context::flags;
}
#endif //MREGEX_REGEX_CONTEXT_HPP