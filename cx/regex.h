#ifndef CX_REGEX_H
#define CX_REGEX_H

#include "parser.h"
#include "regex_base.h"

namespace cx
{
    template<static_string const pattern, typename ... Flags>
    struct regex : regex_base<get_ast<pattern>, Flags ...>
    {
        static_assert(parser<pattern>::accepted, "syntax error in regular expression");
    };
}
#endif //CX_REGEX_H