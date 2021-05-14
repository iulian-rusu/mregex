#ifndef CTR_UTILITY_H
#define CTR_UTILITY_H
#include <type_traits>

namespace ctr
{
    // some utility metafunctions

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

    template<typename First, typename Second>
    struct pair
    {
        using first = First;
        using second = Second;
    };
}

#endif //CTR_UTILITY_H
