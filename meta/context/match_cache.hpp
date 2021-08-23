#ifndef META_MATCH_CACHE_HPP
#define META_MATCH_CACHE_HPP

#include <array>

namespace meta
{
    struct cache_entry
    {
        std::size_t from{};
        std::size_t consumed{};
    };

    template<bool enable>
    struct match_cache
    {
        static constexpr std::size_t capacity = 256;

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return size == 0;
        }

        [[nodiscard]] constexpr cache_entry top() const noexcept
        {
            return data[index];
        }

        constexpr void push(cache_entry entry) noexcept
        {
            index = (index < capacity - 1) * (index + 1);
            ++size;
            data[index] = entry;
        }

        constexpr void pop() noexcept
        {
            index = (index == 0) ? 9 : index - 1;
            --size;
        }
    private:
        std::array<cache_entry, capacity> data{};
        std::size_t index{};
        std::size_t size{};
    };

    template<>
    struct match_cache<false>
    {
        // Used when the cache flag is not enabled
    };
}
#endif //META_MATCH_CACHE_HPP
