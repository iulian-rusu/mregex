#ifndef META_MATCH_CACHE_HPP
#define META_MATCH_CACHE_HPP

#include <array>
#include <concepts>

namespace meta
{
    template<std::forward_iterator Iter>
    struct cache_entry
    {
        Iter from{};
        std::size_t consumed{};
    };

    template<std::forward_iterator Iter, std::size_t N>
    struct match_cache
    {
        using value_type = cache_entry<Iter>;

        static constexpr std::size_t capacity = N;

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return size == 0;
        }

        [[nodiscard]] constexpr value_type top() const noexcept
        {
            return data[index];
        }

        constexpr void push(value_type entry) noexcept
        {
            index = (index < capacity - 1) * (index + 1);
            ++size;
            data[index] = entry;
        }

        constexpr void pop() noexcept
        {
            index = (index == 0) ? capacity - 1 : index - 1;
            --size;
        }

        constexpr void clear() noexcept
        {
            index = 0;
            size = 0;
        }
    private:
        std::array<value_type, capacity> data{};
        std::size_t index{};
        std::size_t size{};
    };
}
#endif //META_MATCH_CACHE_HPP
