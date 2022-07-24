#ifndef MREGEX_XPR_FLAGS_HPP
#define MREGEX_XPR_FLAGS_HPP

#include <mregex/regex_interface.hpp>

namespace meta::xpr
{
    namespace flag
    {
        inline constexpr auto icase = meta::flag::icase{};
        inline constexpr auto dotall = meta::flag::dotall{};
        inline constexpr auto multiline = meta::flag::multiline{};
        inline constexpr auto ungreedy = meta::flag::ungreedy{};
        inline constexpr auto unroll = meta::flag::unroll{};
    }

    /**
     * Sets a sequence of flags for a given regular expression.
     */
    template<typename... Ts, typename... Flags>
    constexpr auto set(regex_interface<Ts ...>, Flags...) noexcept -> regex_interface<Ts ..., Flags ...> { return {}; }

    /**
     * Unsets all flags on a given regular expression.
     */
    template<typename AST, typename... Flags>
    constexpr auto unset(regex_interface<AST, Flags ...>) noexcept -> regex_interface<AST> { return {}; }
}
#endif //MREGEX_XPR_FLAGS_HPP