#ifndef MREGEX_XPR_FLAGS_HPP
#define MREGEX_XPR_FLAGS_HPP

#include <mregex/regex.hpp>

namespace meta::xpr
{
    /**
     * Adds a sequence of flags to a given regular expression.
     */
    template<regex_flag... Flags, typename Regex>
    constexpr auto add_flags(Regex) noexcept -> typename Regex::template add_flags<Flags ...> { return {}; }

    /**
     * Clears all flags from a given regular expression.
     */
    template<typename Regex>
    constexpr auto clear_flags(Regex) noexcept -> typename Regex::clear_flags { return {}; }
}
#endif //MREGEX_XPR_FLAGS_HPP