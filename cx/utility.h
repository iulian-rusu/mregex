#ifndef CX_UTILITY_H
#define CX_UTILITY_H
#include <type_traits>

namespace cx
{
    // some utility structs and metafunctions
    template <class T>
    concept string_like = requires(T a, std::size_t index)
    {
        static_cast<char>(a[index]);
        static_cast<std::size_t>(a.length());
        a.begin();
        a.end();
    };

    template<typename First, typename Second>
    struct pair
    {
        using first = First;
        using second = Second;
    };

    template<typename First, typename ...>
    struct first
    {
        using type = First;
    };

    template<typename ... Elems>
    using first_t = typename first<Elems ...>::type;

    template<typename First, typename ... Rest>
    struct last
    {
        using type = typename last<Rest ...>::type;
    };

    template<typename First>
    struct last<First>
    {
        using type = First;
    };

    template<typename ... Elems>
    using last_t = typename last<Elems ...>::type;

    template<typename First, typename ... Rest>
    struct count_captures
    {
        static constexpr std::size_t capture_count = First::capture_count + count_captures<Rest ...>::capture_count;
    };

    template<typename First>
    struct count_captures<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;
    };
}

#endif //CX_UTILITY_H