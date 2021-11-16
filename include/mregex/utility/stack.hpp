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
        template<typename, typename... Rest>
        struct pop_helper
        {
            using type = stack<Rest ...>;
        };

        template<typename... Elems>
        using pop = typename pop_helper<Elems ...>::type;

        template<typename First, typename... Rest>
        struct push_helper
        {
            using type = stack<First, Rest ...>;
        };

        template<typename... First , typename... Rest>
        struct push_helper<stack<First ...>, Rest ...>
        {
            using type = stack<First ..., Rest ...>;
        };

        template<typename First, typename... Rest>
        using push = typename push_helper<First, Rest ...>::type;
    }

    template<typename... Elems>
    struct stack
    {
        template<typename E>
        using push = detail::push<E, Elems ...>;

        using pop = detail::pop<Elems ...>;

        using top = first_t<Elems ...>;
    };

    struct empty_stack_marker {};

    template<>
    struct stack<>
    {
        template<typename E>
        using push = detail::push<E>;

        using pop = stack<>;

        using top = empty_stack_marker;
    };

    template<typename Stack>
    inline constexpr bool is_empty_v = std::is_same_v<empty_stack_marker, typename Stack::top>;

    template<typename Stack, typename E>
    using push = typename Stack::template push<E>;

    template<typename Stack>
    using pop = typename Stack::pop;

    template<typename Stack>
    using top = typename Stack::top;
}
#endif //MREGEX_STACK_HPP