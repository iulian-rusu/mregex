#ifndef CX_MATCH_CONTEXT_H
#define CX_MATCH_CONTEXT_H

#include "utility/tuple_helpers.h"
#include "regex_flags.h"
#include "ast/capture.h"

namespace cx
{
    /**
     * Creates a flag-depended cx::match_context
     *
     * @tparam Flags    A variable pack of cx::flag types
     * @see regex_flags.h
     */
    template<typename ... Flags>
    struct match_context_factory
    {
        /**
         * Data structure associated with matching/searching.
         * Holds information like capture groups and Regex flags
         *
         * @tparam Regex    The type of Regex used to match the input
         */
        template<typename Regex>
        struct match_context
        {
            struct flags
            {
                static constexpr bool ignore_case = is_any_of_v<flag::ignore_case, Flags ...>;
                static constexpr bool dotall = is_any_of_v<flag::dotall, Flags ...>;
                static constexpr bool multiline = is_any_of_v<flag::multiline, Flags ...>;
                static constexpr bool extended = is_any_of_v<flag::extended, Flags ...>;
                static constexpr bool greedy_alt = is_any_of_v<flag::greedy_alt, Flags ...>;
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
    using create_match_context_t = typename match_context_factory<Flags ...>::template match_context<Regex>;
}
#endif //CX_MATCH_CONTEXT_H
