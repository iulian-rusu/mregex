#ifndef MREGEX_UTILITY_TUPLE_HPP
#define MREGEX_UTILITY_TUPLE_HPP

#include <functional>
#include <tuple>

namespace meta
{
    namespace detail
    {
        template<typename Tuple, typename Func, std::size_t... Indices>
        constexpr auto transform_tuple(Tuple const &tuple, Func &&func, std::index_sequence<Indices ...>)
        {
            return std::tuple{std::invoke(func, std::get<Indices>(tuple)) ...};
        }

        template<typename Tuple, typename Func, std::size_t... Indices>
        constexpr void iterate_tuple(Tuple &&tuple, Func &&func, std::index_sequence<Indices ...>)
        {
            (std::invoke(func, std::get<Indices>(std::forward<Tuple>(tuple))), ...);
        }
    }

    /**
     * Creates a new tuple by applying an invocable object on each element of
     * another tuple.
     *
     * @param tuple     The original tuple
     * @param func      The function invoked to transform each tuple element
     * @return          A new std::tuple that contains the generated elements
     */
    template<typename Tuple, typename Func>
    [[nodiscard]] constexpr auto transform_tuple(Tuple const &tuple, Func &&func)
    {
        return detail::transform_tuple(
            tuple,
            std::forward<Func>(func),
            std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{}
        );
    }

    /**
     * Iterates a tuple-like container and invokes a function on each element.
     *
     * @param tuple     The tuple to be iterated
     * @param func      The function invoked on each tuple element
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
}
#endif //MREGEX_UTILITY_TUPLE_HPP