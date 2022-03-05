#ifndef MREGEX_STACK_HPP
#define MREGEX_STACK_HPP

#include <type_traits>
#include <mregex/utility/meta_helpers.hpp>

namespace meta
{
    template<typename...>
    struct stack;

    namespace detail
    {
        template<typename T, typename... Elems>
        struct push_helper
        {
            using type = stack<T, Elems ...>;
        };

        template<typename... Ts , typename... Elems>
        struct push_helper<stack<Ts ...>, Elems ...>
        {
            using type = stack<Ts ..., Elems ...>;
        };

        template<typename T, typename... Elems>
        using push = typename push_helper<T, Elems ...>::type;
    }

    template<typename First, typename... Rest>
    struct stack<First, Rest ...>
    {
        template<typename T>
        using push = detail::push<T, First, Rest ...>;

        using pop = stack<Rest ...>;

        using top = First;
    };

    struct empty_stack_marker {};

    template<>
    struct stack<>
    {
        template<typename T>
        using push = detail::push<T>;

        using pop = stack<>;

        using top = empty_stack_marker;
    };

    template<typename Stack, typename T>
    using push = typename Stack::template push<T>;

    template<typename Stack>
    using pop = typename Stack::pop;

    template<typename Stack>
    using top = typename Stack::top;

    template<typename Stack>
    inline constexpr bool is_empty_v = std::is_same_v<empty_stack_marker, typename Stack::top>;

}
#endif //MREGEX_STACK_HPP