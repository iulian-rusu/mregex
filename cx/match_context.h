#ifndef CX_MATCH_CONTEXT_H
#define CX_MATCH_CONTEXT_H

#include "utility/tuple_helpers.h"
#include "regex_flags.h"

namespace cx
{
    /**
     * Contains a cx::match_context dependent on a Regex and matching flags
     *
     * @tparam Regex    The type of Regex used to match the input
     * @tparam Flags    A variable pack of cx::flag types
     */
    template<typename Regex, typename ... Flags>
    struct match_context_factory
    {
        /**
         * Data structure associated with matching/searching.
         * Holds information like capture groups and Regex flags
         */
        struct match_context
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
                map_tuple(captures, [](auto &capture) {
                    capture.reset();
                });
            }
        };
    };

    template<typename Regex, typename ... Flags>
    using create_match_context = typename match_context_factory<Regex, Flags ...>::match_context;
}
#endif //CX_MATCH_CONTEXT_H
