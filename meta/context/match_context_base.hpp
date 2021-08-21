#ifndef META_MATCH_CONTEXT_BASE_HPP
#define META_MATCH_CONTEXT_BASE_HPP

#include "../utility/tuple_helpers.hpp"
#include "../regex_flags.hpp"

namespace meta
{
    template<typename AST, typename ... Flags>
    struct match_context_base
    {
        struct flags
        {
            static constexpr bool ignore_case = is_flag_enabled_v<flag::ignore_case, Flags ...>;
            static constexpr bool dotall = is_flag_enabled_v<flag::dotall, Flags ...>;
            static constexpr bool multiline = is_flag_enabled_v<flag::multiline, Flags ...>;
            static constexpr bool greedy_alt = is_flag_enabled_v<flag::greedy_alt, Flags ...>;
        };

        regex_capture_view_storage<AST::capture_count> captures{};

        constexpr void clear_captures() noexcept
        {
            tuple_for_each(captures, [](auto &capture) {
                capture.clear();
            });
        }
    };
}
#endif //META_MATCH_CONTEXT_BASE_HPP
