#ifndef MREGEX_GRAMMAR_BACKREFS_HPP
#define MREGEX_GRAMMAR_BACKREFS_HPP

#include <mregex/grammar/actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that handles any escape sequence which marks the beginning
     * of a backreference or a named backreference.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct begin_backref
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::backref_id<C - '0'>
                >;
    };

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
     * Metafunction that decides if the current rule will continue
     * the backreference ID building or will finish it.
     *
     * @tparam ID   The current backreference ID on the stack
     * @tparam C    The current character in the input pattern
     */
    template<std::size_t ID, char C, bool = is_numeric<C>>
    struct update_backref
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::backref_id<10 * ID + C - '0'>
                >;
    };

    template<std::size_t ID, char C>
    struct update_backref<ID, C, false>
    {
        using type = symbol::make_backref<ID>;
    };

    template<std::size_t ID, char C>
    using update_backref_t = typename update_backref<ID, C>::type;
}
#endif //MREGEX_GRAMMAR_BACKREFS_HPP