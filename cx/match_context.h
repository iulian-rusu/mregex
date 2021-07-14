#ifndef CX_MATCH_CONTEXT_H
#define CX_MATCH_CONTEXT_H

#include "utility/tuple_helpers.h"
#include "regex_flags.h"
#include "ast/capture.h"

namespace cx
{
    template<typename Regex>
    struct match_context
    {
        using flags = resolve_flags<Regex>;

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
