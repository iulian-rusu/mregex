#ifndef META_MATCH_RESULT_HPP
#define META_MATCH_RESULT_HPP

namespace meta::ast
{
    /**
     * Data structure returned by all matching functions of AST nodes.
     */
    struct match_result
    {
        std::size_t consumed{};
        bool matched{};

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }

        constexpr match_result operator+(match_result const &other) const noexcept
        {
            return match_result{consumed + other.consumed, matched || other.matched};
        }

        constexpr match_result &operator+=(match_result const &other) noexcept
        {
            consumed += other.consumed;
            matched = matched || other.matched;
            return *this;
        }
    };
}
#endif //META_MATCH_RESULT_HPP