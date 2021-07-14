#ifndef CX_MATCH_CONTEXT_H
#define CX_MATCH_CONTEXT_H

#include "utility/tuple_helpers.h"
#include "regex_flags.h"
#include "ast/capture.h"

namespace cx
{
    /**
     * Data structure containing information necessary to match a Regex
     *
     * @tparam Regex    The cx::regex type used to match the input
     */
    template<typename Regex>
    struct match_context
    {
        capture_storage<Regex::capture_count> captures{};

        constexpr void clear_captures() noexcept
        {
            map_tuple(captures, [](auto &capture) {
                capture.reset();
            });
        }
    };
}
#endif //CX_MATCH_CONTEXT_H
