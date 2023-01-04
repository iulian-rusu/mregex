#ifndef MREGEX_UTILITY_TUPLE_HPP
#define MREGEX_UTILITY_TUPLE_HPP

#include <tuple>

namespace meta
{
    namespace detail
    {
        template<typename Tuple, typename Func, std::size_t... Indices>
        constexpr auto transform_tuple(Tuple const &tuple, Func &&func, std::index_sequence<Indices ...>)
        {
            return std::tuple{func(std::get<Indices>(tuple)) ...};
        }

        template<typename Tuple, typename Func, std::size_t... Indices>
        constexpr void iterate_tuple(Tuple &&tuple, Func &&func, std::index_sequence<Indices ...>)
        {
            (func(std::get<Indices>(std::forward<Tuple>(tuple))), ...);
        }

        template<typename Result, typename Func, std::size_t... Indices>
        constexpr auto transform_groups(Result const &result, Func &&func, std::index_sequence<Indices ...>)
        {
            return std::tuple{func(result.template group<Indices + 1>()) ...};
        }

        template<typename Result, typename Func, std::size_t... Indices>
        constexpr void iterate_groups(Result &&result, Func &&func, std::index_sequence<Indices ...>)
        {
            (func(std::forward<Result>(result).template group<Indices + 1>()), ...);
        }
    }

    /**
     * Creates a new tuple by applying a callable object on each element of
     * another tuple.
     *
     * @param tuple     The original tuple
     * @param func      The callable that generates elements for the new tuple
     * @return          A new std::tuple that contains the generated elements
     */
    template<typename Tuple, typename Func>
    constexpr auto transform_tuple(Tuple const &tuple, Func &&func)
    {
        return detail::transform_tuple(
                tuple,
                std::forward<Func>(func),
                std::make_index_sequence<std::tuple_size_v<Tuple>>{}
        );
    }

    /**
     * Invokes a callable object for each element of a tuple-like container.
     *
     * @param tuple     The tuple to be iterated
     * @param func      The function called on each tuple element
     */
    template<typename Tuple, typename Func>
    constexpr void iterate_tuple(Tuple &&tuple, Func &&func)
    {
        detail::iterate_tuple(
            std::forward<Tuple>(tuple),
            std::forward<Func>(func),
            std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{}
        );
    }

    /**
     * Creates a new tuple by applying a callable object on each group of
     * a tuple-like regex result container.
     *
     * @param result    The original regex result container
     * @param func      The callable that generates elements for the new tuple
     * @return          A new std::tuple that contains the generated elements
     */
    template<typename Result, typename Func>
    constexpr auto transform_groups(Result const &result, Func &&func)
    {
        return detail::transform_groups(
                result,
                std::forward<Func>(func),
                std::make_index_sequence<std::tuple_size_v<Result>>{}
        );
    }

    /**
     * Invokes a callable object for each group of a tuple-like regex result container.
     *
     * @param result    The regex result container to be iterated
     * @param func      The function called on each group
     */
    template<typename Result, typename Func>
    constexpr void iterate_groups(Result &&result, Func &&func)
    {
        detail::iterate_groups(
            std::forward<Result>(result),
            std::forward<Func>(func),
            std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Result>>>{}
        );
    }
}
#endif //MREGEX_UTILITY_TUPLE_HPP