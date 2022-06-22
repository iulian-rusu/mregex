#ifndef MREGEX_LOOKBEHINDS_HPP
#define MREGEX_LOOKBEHINDS_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    template<typename Inner>
    struct positive_lookbehind
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            static_assert(std::bidirectional_iterator<Iter>, "lookbehinds require bidirectional iterators");
            // TODO: implement positive lookbehind
            return cont(it);
        }
    };

    template<typename Inner>
    struct negative_lookbehind
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            static_assert(std::bidirectional_iterator<Iter>, "lookbehinds require bidirectional iterators");
            // TODO: implement negative lookbehind
            return cont(it);
        }
    };
}
#endif //MREGEX_LOOKBEHINDS_HPP