#ifndef MREGEX_REGEX_CONTEXT_HPP
#define MREGEX_REGEX_CONTEXT_HPP

#include <mregex/utility/tuple.hpp>
#include <mregex/regex_flags.hpp>
#include <mregex/regex_result.hpp>

namespace meta
{
    /**
     * Data structure that holds static information about the regex as well as
     * match-time information like captured contents.
     */
    template<std::forward_iterator Iter, typename AST, typename... Flags>
    struct regex_context
    {
        static_assert((is_flag<Flags> && ...), "invalid flag");

        using ast_type = AST;
        using iterator_type = Iter;
        using result_type = regex_result_view<ast::capture_name_spec_t<ast_type>, iterator_type>;
        using storage_type = typename result_type::storage_type;

        struct flags
        {
            static constexpr bool icase = is_flag_enabled<flag::icase, Flags ...>;
            static constexpr bool dotall = is_flag_enabled<flag::dotall, Flags ...>;
            static constexpr bool multiline = is_flag_enabled<flag::multiline, Flags ...>;
            static constexpr bool ungreedy = is_flag_enabled<flag::ungreedy, Flags ...>;
            static constexpr bool unroll = is_flag_enabled<flag::unroll, Flags ...>;
        };

        storage_type captures{};

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