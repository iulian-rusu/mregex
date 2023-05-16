#ifndef MREGEX_REGEX_HPP
#define MREGEX_REGEX_HPP

#include <mregex/parser/parser.hpp>
#include <mregex/regex_interface.hpp>

namespace meta
{
    template<static_string Pattern>
    constexpr bool assert_correct_syntax() noexcept
    {
        static_assert(std::is_same_v<parser_verdict<Pattern>, parsing::success>, "syntax error in regular expression");
        return true;
    }

    /**
     * Creates a regex type from a given literal string.
     *
     * @tparam Pattern  The regex defined as a string
     * @tparam Flags    Optional flags for matching
     */
    template<static_string Pattern, regex_flag... Flags>
    requires (assert_correct_syntax<Pattern>())
    struct regex : regex_interface<ast_of<Pattern>, Flags ...>
    {
        using base_type = regex_interface<ast_of<Pattern>, Flags ...>;
    };
}
#endif //MREGEX_REGEX_HPP