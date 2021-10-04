#ifndef MREGEX_REGEX_HPP
#define MREGEX_REGEX_HPP

#include <mregex/parser.hpp>
#include <mregex/regex_base.hpp>

namespace meta
{
    template<static_string pattern, typename... Flags>
    struct regex : regex_base<ast_of<pattern>, Flags ...>
    {
        static constexpr auto error_type = identify_error<error_of<pattern>>::error_type;
    };
}
#endif //MREGEX_REGEX_HPP