#ifndef MREGEX_EXPR_FLAGS_HPP
#define MREGEX_EXPR_FLAGS_HPP

#include <mregex/regex_flags.hpp>

namespace meta::expr
{
    namespace flag
    {
        inline constexpr auto icase = meta::flag::icase{};
        inline constexpr auto dotall = meta::flag::dotall{};
        inline constexpr auto multiline = meta::flag::multiline{};
        inline constexpr auto ungreedy = meta::flag::ungreedy{};
        inline constexpr auto unroll = meta::flag::unroll{};
    }

    template<typename... Ts, typename... Flags>
    constexpr auto modify(regex_interface<Ts ...>, Flags...) noexcept -> regex_interface<Ts ..., Flags ...> { return {}; }
}
#endif //MREGEX_EXPR_FLAGS_HPP