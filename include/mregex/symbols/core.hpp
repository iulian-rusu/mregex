#ifndef MREGEX_SYMBOLS_CORE_HPP
#define MREGEX_SYMBOLS_CORE_HPP

#include <cstdint>

namespace meta::symbol
{
    /**
     * Core symbols used in grammar definitions, parsing, AST and matching.
     */

    // Symbols for parsing tokens
    template<char C>
    struct character {};

    struct empty {};

    template<char C>
    struct expect {};

    // Symbols for parsing basic operators
    struct begin {};
    struct esc {};
    struct seq {};
    struct alt {};
    struct alt_seq {};
    struct mod {};

    // Symbols for parsing groups
    struct group_begin_or_mod {};
    struct group_mod {};
    struct group_mod_less_than {};
    struct group_begin {};

    // Symbols for parsing sets
    struct set_begin_or_neg {};
    struct set_begin {};
    struct set_seq {};
    struct set_esc {};
    struct set_range_begin {};
    struct set_range_seq {};
    struct set_range_esc {};

    // Symbol for parsing backreferences
    struct backref_name_begin {};

    template<std::size_t ID>
    struct backref_id {};

    template<char... Chars>
    struct backref_name_seq {};

    // Symbols for parsing named captures
    template<char... Chars>
    struct capture_name_seq {};

    // Symbols for parsing hexadecimal escape sequences
    template<std::uint8_t... Hexdigits>
    struct hex_esc_seq {};
}
#endif //MREGEX_SYMBOLS_CORE_HPP