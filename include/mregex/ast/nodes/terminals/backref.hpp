#ifndef MREGEX_NODES_BACKREF_HPP
#define MREGEX_NODES_BACKREF_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
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
    struct backref_base : terminal, zero_length_matcher
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto const captured = LookupMethod::get_capture(ctx.captures);
            std::size_t const length_to_match = captured.length();
            if (distance_less_than(length_to_match, it, end))
                return {it, false};

            // Lookbehinds use reverse iterators. To check for equality, iterator directions must be the same
            constexpr bool different_iterators = !std::is_same_v<Iter, typename Context::iterator_type>;
            for (auto c : reverse_if<different_iterators>(captured))
            {
                auto subject = *it;
                if constexpr (Context::flags::icase)
                {
                    subject = to_lower(subject);
                    c = to_lower(c);
                }
                if (subject != c)
                    return {it, false};
                ++it;
            }
            return cont(it);
        }

    private:
        template<bool Reverse, typename Range>
        static constexpr auto reverse_if(Range &range) noexcept
        {
            if constexpr (Reverse)
                return reversed_range_view{range};
            else
                return range;
        }
    };

    template<std::size_t ID>
    struct backref : backref_base<detail::id_lookup_method<ID>> {};

    template<typename Name>
    struct named_backref : backref_base<detail::name_lookup_method<Name>> {};
}
#endif //MREGEX_NODES_BACKREF_HPP