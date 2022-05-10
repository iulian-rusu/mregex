#ifndef MREGEX_LEXER_HPP
#define MREGEX_LEXER_HPP

#include <mregex/utility/static_string.hpp>
#include <mregex/symbol.hpp>

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
        template<std::size_t I, bool = I < Pattern.length()>
        struct token
        {
            using type = symbol::character<Pattern[I]>;
        };

        template<std::size_t I>
        struct token<I, false>
        {
            using type = symbol::epsilon;
        };

        template<std::size_t I>
        using token_t = typename token<I>::type;
    };
}
#endif //MREGEX_LEXER_HPP