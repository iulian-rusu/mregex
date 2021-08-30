#ifndef META_REGEX_CONTEXT_HPP
#define META_REGEX_CONTEXT_HPP

#include "regex_flags.hpp"
#include "utility/tuple_helpers.hpp"

namespace meta
{
    template<std::forward_iterator Iter, typename AST, typename... Flags>
    struct regex_context_factory
    {
        static_assert((is_flag_v<Flags> && ...), "invalid flag");

        struct regex_context
        {
            struct flags
            {
                static constexpr bool ignore_case = is_flag_enabled_v<flag::ignore_case, Flags ...>;
                static constexpr bool dotall = is_flag_enabled_v<flag::dotall, Flags ...>;
                static constexpr bool multiline = is_flag_enabled_v<flag::multiline, Flags ...>;
                static constexpr bool ungreedy = is_flag_enabled_v<flag::ungreedy, Flags ...>;
            };

            using ast_type = AST;
            using iterator_type = Iter;

            regex_capture_view_storage<AST::capture_count, Iter> captures{};

            constexpr void clear() noexcept
            {
                tuple_for_each(captures, [](auto &capture) {
                    capture.clear();
                });
            }
        };
    };

    template<std::forward_iterator Iter, typename AST, typename... Flags>
    using create_regex_context = typename regex_context_factory<Iter, AST, Flags ...>::regex_context;

    template<typename Context>
    using flags = typename Context::flags;
}
#endif //META_REGEX_CONTEXT_HPP