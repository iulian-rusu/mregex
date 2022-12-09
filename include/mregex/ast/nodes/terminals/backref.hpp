#ifndef MREGEX_NODES_BACKREF_HPP
#define MREGEX_NODES_BACKREF_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/utility/distance.hpp>
#include <mregex/utility/reversed_range_view.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<std::size_t ID>
        struct id_lookup_method
        {
            template<capture_storage Storage>
            static constexpr decltype(auto) get_capture(Storage &captures) noexcept
            {
                return std::get<ID>(captures);
            }
        };

        template<typename Name>
        struct name_lookup_method
        {
            template<capture_storage Storage>
            static constexpr decltype(auto) get_capture(Storage &captures) noexcept
            {
                return std::get<named_capture_type_for<Storage, Name>>(captures);
            }
        };
    }

    template<typename LookupMethod>
    struct backref_impl : terminal
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
                if constexpr (context_flags<Context>::icase)
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
    struct backref : backref_impl<detail::id_lookup_method<ID>> {};

    template<typename Name>
    struct named_backref : backref_impl<detail::name_lookup_method<Name>> {};
}
#endif //MREGEX_NODES_BACKREF_HPP