#ifndef CX_REGEX_FLAGS_H
#define CX_REGEX_FLAGS_H

#include "utility/meta_helpers.h"

namespace cx
{
    namespace flag
    {
        struct ignore_case {};
        struct dotall {};
        struct multiline {};
        struct extended {};
        struct greedy_alt {};
    }

    template<typename Regex>
    using flags = typename Regex::flags;
}
#endif //CX_REGEX_FLAGS_H
