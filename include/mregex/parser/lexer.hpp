#ifndef MREGEX_PARSER_LEXER_HPP
#define MREGEX_PARSER_LEXER_HPP

#include <mregex/symbols/core.hpp>
#include <mregex/utility/static_string.hpp>

namespace meta
{
    /**
     * Compile-time lexer that extracts tokens from the input pattern.
     *
     * @tparam Pattern  The input string to be tokenized
     */
    template<static_string Pattern>
    struct lexer
    {
        template<std::size_t I>
        struct token
        {
            using type = symbol::empty;
        };

        template<std::size_t I>
        requires (I < Pattern.length())
        struct token<I>
        {
            using type = symbol::character<Pattern[I]>;
        };

        template<std::size_t I>
        using token_t = typename token<I>::type;
    };
}
#endif //MREGEX_PARSER_LEXER_HPP