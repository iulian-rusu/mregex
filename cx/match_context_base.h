#ifndef CX_MATCH_CONTEXT_BASE_H
#define CX_MATCH_CONTEXT_BASE_H

#include "utility/tuple_helpers.h"
#include "regex_flags.h"

namespace cx
{
    template<typename Regex, typename ... Flags>
    struct match_context_base
    {
        struct flags
        {
            static constexpr bool ignore_case = check_flag_v<flag::ignore_case, Flags ...>;
            static constexpr bool dotall = check_flag_v<flag::dotall, Flags ...>;
            static constexpr bool multiline = check_flag_v<flag::multiline, Flags ...>;
            static constexpr bool extended = check_flag_v<flag::extended, Flags ...>;
            static constexpr bool greedy_alt = check_flag_v<flag::greedy_alt, Flags ...>;
        };

        capture_storage<Regex::capture_count> captures{};

        constexpr void clear_captures() noexcept
        {
            tuple_map(captures, [](auto &capture) {
                capture.reset();
            });
        }
    };
}
#endif //CX_MATCH_CONTEXT_BASE_H
