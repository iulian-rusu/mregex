#ifndef MREGEX_GRAMMAR_BACKREFS_HPP
#define MREGEX_GRAMMAR_BACKREFS_HPP

#include <mregex/grammar/actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that defines symbols used to begin parsing a backreference ID.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct begin_backref_id
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::backref_id<C - '0'>
                >;
    };

    /**
     * Metafunction that defines symbols used to begin parsing a backreference name.
     */
    struct begin_named_backref
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::expect<'<'>,
                    symbol::backref_name_begin
                >;
    };

    /**
     * Metafunction that defines symbols used to continue parsing a backreference ID.
     *
     * @tparam ID   The backreference ID being parsed
     * @tparam C    The current character in the input pattern
     */
    template<std::size_t ID, char C>
    struct continue_backref_id
    {
        using type = symbol::make_backref<ID>;
    };

    template<std::size_t ID, char C>
    requires (is_digit(C))
    struct continue_backref_id<ID, C>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::backref_id<10 * ID + C - '0'>
                >;
    };

    template<std::size_t ID, char C>
    using continue_backref_id_t = typename continue_backref_id<ID, C>::type;
}
#endif //MREGEX_GRAMMAR_BACKREFS_HPP