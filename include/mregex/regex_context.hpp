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
    template<std::forward_iterator Iter, typename AST, regex_flags flags>
    struct regex_context
    {
        struct flags_type
        {
            static constexpr bool ignore_case = flag::icase & flags;
            static constexpr bool dotall = flag::dotall & flags;
            static constexpr bool multiline = flag::multiline & flags;
            static constexpr bool ungreedy = flag::ungreedy & flags;
            static constexpr bool unroll = flag::unroll & flags;
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
    using flags_of = typename Context::flags_type;
}
#endif //MREGEX_REGEX_CONTEXT_HPP