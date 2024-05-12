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
     */
    template<char... Chars>
    using make_name = name<make_static_string<Chars ...>()>;
}
#endif //MREGEX_SYMBOLS_NAMES_HPP