#ifndef CX_UTILITY_H
#define CX_UTILITY_H

#include <type_traits>

/**
 * File with various utility metafunctions
 */
namespace cx
{
    template<typename T>
    concept string_like =
    requires(T s, std::size_t index)
    {
        static_cast<char>(s[index]);
        static_cast<std::size_t>(s.length());
        s.begin();
        s.end();
        s.substr(index, index);
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

    template<typename Test, typename First, typename ... Rest>
    struct is_any_of
    {
        static constexpr auto value = std::is_same_v<Test, First> || is_any_of<Test, Rest ...>::value;
    };

    template<typename Test, typename First>
    struct is_any_of<Test, First>
    {
        static constexpr auto value = std::is_same_v<Test, First>;
    };

    template<typename Test, typename First, typename ... Rest>
    constexpr auto is_any_of_v = is_any_of<Test, First, Rest ...>::value;
}

#endif //CX_UTILITY_H
