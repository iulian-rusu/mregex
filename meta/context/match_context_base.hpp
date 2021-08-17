#ifndef META_MATCH_CONTEXT_BASE_HPP
#define META_MATCH_CONTEXT_BASE_HPP

#include "../utility/tuple_map.hpp"
#include "../regex_flags.hpp"

namespace meta
{
    template<typename AST, typename ... Flags>
    struct match_context_base
    {
        struct flags
        {
            static constexpr bool ignore_case = check_flag_v<flag::ignore_case, Flags ...>;
            static constexpr bool dotall = check_flag_v<flag::dotall, Flags ...>;
            static constexpr bool multiline = check_flag_v<flag::multiline, Flags ...>;
            static constexpr bool greedy_alt = check_flag_v<flag::greedy_alt, Flags ...>;
        };

        capture_view_storage<AST::capture_count> captures{};

        constexpr void clear_captures() noexcept
        {
            for_each_tuple_element(captures, [](auto &capture) {
                capture.reset();
            });
        }
    };
}
#endif //META_MATCH_CONTEXT_BASE_HPP
