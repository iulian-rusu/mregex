#ifndef META_STACK_HPP
#define META_STACK_HPP

#include <type_traits>
#include "meta_helpers.hpp"

namespace meta
{
    /**
     * Compile-time stack implementation.
     *
     * @tparam Elems    Inner variadic pack of types - the elemets on the stack
     */
    template<typename ... Elems>
    struct stack;

    /**
     * Namespace with helper metafunctions for stack manipulation.
     */
    namespace impl
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
        using push = impl::push<E, Elems ...>;

        using pop = impl::template pop<Elems ...>;

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
    constexpr bool is_empty_v = std::is_same_v<empty_stack_marker, typename Stack::top>;

    template<typename Stack>
    using pop = typename Stack::pop;

    template<typename Stack>
    using top = typename Stack::top;
}
#endif //META_STACK_HPP