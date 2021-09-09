#ifndef META_STACK_HPP
#define META_STACK_HPP

#include <type_traits>
#include "meta_helpers.hpp"

namespace meta
{
    template<typename...>
    struct stack;

    /**
     * Namespace with helper metafunctions for stack manipulation.
     */
    namespace impl
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
        using push = impl::push<E, Elems ...>;

        using pop = impl::pop<Elems ...>;

        using top = first_t<Elems ...>;
    };

    struct empty_stack_marker {};

    template<>
    struct stack<>
    {
        template<typename E>
        using push = impl::push<E>;

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
#endif //META_STACK_HPP