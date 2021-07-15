#ifndef CX_STACK_H
#define CX_STACK_H

#include <type_traits>

namespace cx
{
    /**
     * Compile-time stack implementation
     *
     * @tparam Elems    Inner variadic pack of types - the elemets on the stack
     */
    template<typename ... Elems>
    struct stack
    {
        /**
         * Helper for specializing top, pop and push metafunctions
         */
        struct stack_helper
        {
            template<typename First, typename ...>
            using top = First;

            template<typename, typename ... Rest>
            using pop = stack<Rest ...>;

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
        };

        template<typename E>
        using push = typename stack_helper::template push<E, Elems ...>;

        using top = typename stack_helper::template top<Elems ...>;

        using pop = typename stack_helper::template pop<Elems ...>;
    };

    struct empty_stack {};

    template<>
    struct stack<>
    {
        struct stack_helper
        {
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
        };

        template<typename E>
        using push = typename stack_helper::template push<E>;

        using top = empty_stack;

        using pop = stack<>;
    };

    template<typename Stack>
    constexpr bool is_empty_v = std::is_same_v<empty_stack, typename Stack::top>;
}
#endif //CX_STACK_H