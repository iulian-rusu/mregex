#ifndef CX_MATCH_CONTEXT_H
#define CX_MATCH_CONTEXT_H

#include "capture.h"
#include "va_helpers.h"

/**
 * File with the data structure for tracking the context during Regex matching
 */
namespace cx
{
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
