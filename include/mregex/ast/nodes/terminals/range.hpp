#ifndef MREGEX_NODES_RANGE_HPP
#define MREGEX_NODES_RANGE_HPP

#include <mregex/ast/nodes/terminals/literal.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<typename R1, typename R2>
        struct range_intersection;

        template<char A1, char B1, char A2, char B2>
        struct range_intersection<range<A1, B1>, range<A2, B2>>
        {
            static constexpr char start = A1 > A2 ? A1 : A2;
            static constexpr char stop = B1 < B2 ? B1 : B2;
            static constexpr bool is_empty = start > stop;

            static constexpr bool contains(char input) noexcept
            {
                return start <= input && input <= stop;
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
        static_assert(A < B, "invalid range bounds");

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter it, Context &) noexcept
        {
            auto input = *it;
            bool result = A <= input && input <= B;
            if constexpr (Context::flags::icase)
                result |= is_in_alpha_subrange(flip_case_bit(input));
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