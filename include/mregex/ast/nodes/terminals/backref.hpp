#ifndef MREGEX_NODES_BACKREF_HPP
#define MREGEX_NODES_BACKREF_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/utility/distance.hpp>
#include <mregex/utility/reversed_range_view.hpp>
#include <mregex/regex_capture.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<std::size_t ID>
        struct id_lookup_method
        {
            template<capture_storage CaptureStorage>
            static constexpr decltype(auto) get_capture(CaptureStorage &captures) noexcept
            {
                return std::get<ID>(captures);
            }
        };

        template<typename Name>
        struct name_lookup_method
        {
            template<capture_storage CaptureStorage>
            static constexpr decltype(auto) get_capture(CaptureStorage &captures) noexcept
            {
                using capture_type = rename_capture_t<std::tuple_element_t<0, CaptureStorage>, Name>;
                return std::get<capture_type>(captures);
            }
        };
    }

    template<typename LookupMethod>
    struct backref_base : zero_length_matcher
    {
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            auto const captured = LookupMethod::get_capture(ctx.captures);
            std::size_t const length_to_match = captured.length();
            if (distance_less_than(length_to_match, current, end))
                return non_match(current);

            // Lookbehinds use reverse iterators. To check for equality, iterator directions must be the same
            constexpr bool different_iterators = !std::is_same_v<Iter, typename Context::iterator_type>;
            for (auto target : reverse_if<different_iterators>(captured))
            {
                if (!equals<Context::flags::icase>(target, *current))
                    return non_match(current);
                ++current;
            }
            return cont(current);
        }

    private:
        template<bool Reverse, std::ranges::bidirectional_range Range>
        static constexpr auto reverse_if(Range &range) noexcept
        {
            if constexpr (Reverse)
                return reversed_range_view{range};
            else
                return range;
        }

        template<bool IgnoreCase>
        static constexpr bool equals(char a, char b) noexcept
        {
            return a == b || (IgnoreCase && a == to_lower(b));
        }
    };

    template<std::size_t ID>
    struct backref : backref_base<detail::id_lookup_method<ID>> {};

    template<typename Name>
    struct named_backref : backref_base<detail::name_lookup_method<Name>> {};
}
#endif //MREGEX_NODES_BACKREF_HPP