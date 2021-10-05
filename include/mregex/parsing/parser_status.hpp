#ifndef MREGEX_PARSER_STATUS_HPP
#define MREGEX_PARSER_STATUS_HPP

#include <type_traits>

namespace meta::parsing
{
    struct success : std::true_type {}; // Parsing finished with success

    template<std::size_t>
    struct syntax_error : std::false_type {}; // Parsing finished with syntax error
}
#endif //MREGEX_PARSER_STATUS_HPP