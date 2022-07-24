#ifndef MREGEX_PARSER_STATUS_HPP
#define MREGEX_PARSER_STATUS_HPP

#include <type_traits>

namespace meta::parsing
{
    // Parsing finished with success
    struct success : std::true_type {};

    // Parsing finished with syntax error
    template<std::size_t>
    struct syntax_error : std::false_type {};
}
#endif //MREGEX_PARSER_STATUS_HPP