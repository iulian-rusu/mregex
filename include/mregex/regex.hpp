#ifndef MREGEX_REGEX_HPP
#define MREGEX_REGEX_HPP

#include <mregex/parsing/parser.hpp>
#include <mregex/regex_base.hpp>

namespace meta
{
    template<static_string Pattern, typename... Flags>
    struct regex : regex_base<ast_of<Pattern>, Flags ...>
    {
        static_assert(std::is_same_v<status_of<Pattern>, parsing::success>, "syntax error in regular expression");
    };
}
#endif //MREGEX_REGEX_HPP