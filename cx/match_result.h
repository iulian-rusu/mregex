#ifndef CX_MATCH_RESULT_H
#define CX_MATCH_RESULT_H

namespace cx
{
    /**
    * Data structure returned by all matching functions of AST nodes
    */
    struct match_result
    {
        std::size_t count{};
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
            return match_result{count + other.count, matched || other.matched};
        }

        constexpr match_result &operator+=(match_result const &other) noexcept
        {
            count += other.count;
            matched = matched || other.matched;
            return *this;
        }
    };
}
#endif //CX_MATCH_RESULT_H
