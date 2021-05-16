#ifndef CTR_RESULT_H
#define CTR_RESULT_H

/**
 * Defines data structures that encapsulate matching/searching results
 */
namespace ctr
{
    struct match_result
    {
        std::size_t count;
        bool matched;

        constexpr explicit operator bool() const noexcept
        { return matched; }

        constexpr match_result operator+(match_result const &other) const noexcept
        {
            return {count + other.count, matched || other.matched};
        }

        constexpr match_result &operator+=(match_result const &other) noexcept
        {
            count += other.count;
            matched = matched || other.matched;
            return *this;
        }
    };

    struct search_result
    {
        std::size_t start_pos;
        std::size_t count;
        bool matched;

        constexpr search_result(std::size_t start, match_result mr)
                : start_pos(start), count(mr.count), matched(mr.matched) {}
    };
}

#endif //CTR_RESULT_H
