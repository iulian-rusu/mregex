#ifndef META_REGEX_HPP
#define META_REGEX_HPP

#include "parser.hpp"
#include "regex_base.hpp"

namespace meta
{
    template<static_string const pattern, typename ... Flags>
    struct regex : regex_base<ast_of<pattern>, Flags ...>
    {
        static_assert(parser<pattern>::accepted, "syntax error in regular expression");
    };
}
#endif //META_REGEX_HPP