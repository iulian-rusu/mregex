#ifndef MREGEX_SYMBOLS_NAMES_HPP
#define MREGEX_SYMBOLS_NAMES_HPP

#include <mregex/utility/static_string.hpp>

namespace meta::symbol
{
    /**
     * Symbols used to parse and build named AST nodes.
     */

    template<static_string Name>
    struct name
    {
        static constexpr auto value = static_cast<std::string_view>(Name);
    };

    struct unnamed {};

    /**
     * Metafunction that converts a pack of characters to a symbolic name type.
     *
     * @note The sizeof... (Chars) is technically redundant but GCC 11.1 cannot deduce the size.
     */
    template<char... Chars>
    using make_name = name<static_string<sizeof... (Chars)>{{Chars ..., '\0'}}>;
}
#endif //MREGEX_SYMBOLS_NAMES_HPP