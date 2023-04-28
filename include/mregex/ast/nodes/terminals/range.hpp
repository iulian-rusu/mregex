#ifndef MREGEX_NODES_RANGE_HPP
#define MREGEX_NODES_RANGE_HPP

#include <algorithm>
#include <mregex/ast/nodes/terminals/literal.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<typename R1, typename R2>
        struct range_intersection
        {
            static constexpr std::uint8_t start = std::max(R1::start, R2::start);
            static constexpr std::uint8_t stop =  std::min(R1::stop, R2::stop);
            static constexpr bool is_empty = start > stop;

            static constexpr bool contains(char input) noexcept
            {
                return is_in_ascii_range<start, stop>(input);
            }

            static constexpr bool contains(char) noexcept
            requires is_empty
            {
                return false;
            }
        };
    }

    template<char A, char B>
    struct range : trivially_matchable<range<A, B>>
    {
        static constexpr std::uint8_t start = A;
        static constexpr std::uint8_t stop = B;

        static_assert(start < stop, "invalid range bounds");

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &) noexcept
        {
            auto input = *current;
            bool result = is_in_ascii_range<start, stop>(input);
            if constexpr (Context::flags::icase)
                result |= is_in_alpha_subrange(flip_lowercase_bit(input));
            return result;
        }

    private:
        static constexpr bool is_in_alpha_subrange(char input) noexcept
        {
            using this_range = range<A, B>;
            using lower_in_this_range = detail::range_intersection<this_range, lower>;
            using upper_in_this_range = detail::range_intersection<this_range, upper>;

            return lower_in_this_range::contains(input) || upper_in_this_range::contains(input);
        }
    };

    template<char C>
    struct range<C, C> : literal<C> {};
}
#endif //MREGEX_NODES_RANGE_HPP