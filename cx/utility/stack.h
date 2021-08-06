#ifndef CX_STACK_H
#define CX_STACK_H

#include <type_traits>
#include "meta_helpers.h"

namespace cx
{
    /**
     * Compile-time stack implementation
     *
     * @tparam Elems    Inner variadic pack of types - the elemets on the stack
     */
    template<typename ... Elems>
    struct stack;

    /**
     * Namespace with helper metafunctions for stack manipulation
     */
    namespace detail
    {
        template<typename First, typename ... Rest>
        struct pop_helper
        {
            using type = stack<Rest ...>;
        };

        template<typename ... Elems>
        using pop = typename pop_helper<Elems ...>::type;

        template<typename First, typename ... Rest>
        struct push_helper
        {
            using type = stack<First, Rest ...>;
        };

        template<typename ... First , typename ... Rest>
        struct push_helper<stack<First ...>, Rest ...>
        {
            using type = stack<First ..., Rest ...>;
        };

        template<typename First, typename ... Rest>
        using push = typename push_helper<First, Rest ...>::type;
    }

    template<typename ... Elems>
    struct stack
    {
        template<typename E>
        using push = detail::push<E, Elems ...>;

        using pop = detail::template pop<Elems ...>;

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
    constexpr bool is_empty_v = std::is_same_v<empty_stack_marker, typename Stack::top>;
}
#endif //CX_STACK_H