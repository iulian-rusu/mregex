#ifndef MREGEX_TUPLE_HELPERS_HPP
#define MREGEX_TUPLE_HELPERS_HPP

#include <tuple>

namespace meta
{
    namespace detail
    {
        template<typename Tuple, typename Func, std::size_t... Indices>
        constexpr auto generate_tuple(Tuple const &tuple, Func &&func, std::index_sequence<Indices ...> &&)
        {
            return std::tuple{func(std::get<Indices>(tuple)) ...};
        }

        template<typename Tuple, typename Func, std::size_t... Indices>
        constexpr void iterate_tuple(Tuple &&tuple, Func &&func, std::index_sequence<Indices ...> &&)
        {
            (func(std::get<Indices>(tuple)), ...);
        }
    }

    /**
     * Creates a new tuple by applying a callable object on each element of
     * another tuple.
     *
     * @param tuple     The original tuple
     * @param func      The callable that generates elements for the new tuple
     * @return          A new std::tuple with elements returned by mapper
     */
    template<typename Tuple, typename Func>
    constexpr auto generate_tuple(Tuple const &tuple, Func &&func)
    {
        return detail::generate_tuple(
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
}
#endif //MREGEX_TUPLE_HELPERS_HPP