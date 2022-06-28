#ifndef MREGEX_STACK_HPP
#define MREGEX_STACK_HPP

#include <type_traits>
#include <mregex/utility/meta_helpers.hpp>

namespace meta
{
    /*
     * LIFO-style metacontainer for a sequence of types.
     */
    template<typename...>
    struct stack;

    namespace detail
    {
        template<typename Stack, typename T>
        struct push_impl;

        template<typename... Elems, typename T>
        struct push_impl<stack<Elems ...>, T>
        {
            using type = stack<T, Elems ...>;
        };

        template<typename... Elems, typename... Ts>
        struct push_impl<stack<Elems ...>, stack<Ts ...>>
        {
            using type = stack<Ts ..., Elems ...>;
        };

        template<typename Stack>
        struct pop_impl;

        template<typename First, typename... Rest>
        struct pop_impl<stack<First, Rest ...>>
        {
            using type = stack<Rest ...>;
        };

        template<>
        struct pop_impl<stack<>>
        {
            using type = stack<>;
        };
    }

    template<typename First, typename... Rest>
    struct stack<First, Rest ...>
    {
        using top = First;
    };

    struct empty_stack_marker {};

    template<>
    struct stack<>
    {
        using top = empty_stack_marker;
    };

    template<typename Stack, typename T>
    using push = typename detail::push_impl<Stack, T>::type;

    template<typename Stack>
    using pop = typename detail::pop_impl<Stack>::type;

    template<typename Stack>
    using top = typename Stack::top;

    template<typename Stack>
    inline constexpr bool is_empty_v = std::is_same_v<empty_stack_marker, typename Stack::top>;
}
#endif //MREGEX_STACK_HPP