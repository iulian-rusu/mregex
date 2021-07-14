#ifndef CX_REGEX_FLAGS_H
#define CX_REGEX_FLAGS_H

#include "utility/meta_helpers.h"

namespace cx
{
    namespace flag
    {
        struct ignore_case {};
        struct lazy_cycle {};
    }

    template<typename Test, typename ... Flags>
    constexpr bool is_set = is_any_of_v<Test, Flags ...>;

    template<typename>
    struct resolve_flags
    {
        static constexpr bool ignore_case = false;
        static constexpr bool lazy_cycle =  false;
    };

    template<template<auto const, typename ...> typename Regex, auto const pattern, typename ... Flags>
    struct resolve_flags<Regex<pattern, Flags ...>>
    {
        static constexpr bool ignore_case = is_set<flag::ignore_case, Flags ...>;
        static constexpr bool lazy_cycle = is_set<flag::lazy_cycle, Flags ...>;
    };
}
#endif //CX_REGEX_FLAGS_H
