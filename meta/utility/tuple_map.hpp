#ifndef META_TUPLE_MAP_HPP
#define META_TUPLE_MAP_HPP

#include <tuple>

namespace meta
{
    namespace detail
    {
        template<typename Tuple, typename Mapper, std::size_t ... Indices>
        constexpr auto tuple_map(Tuple &&tuple, Mapper &&mapper, std::index_sequence<Indices ...> &&)
        {
            return std::tuple{mapper(std::get<Indices>(tuple)) ... };
        }

        template<typename Tuple, typename Mapper, std::size_t ... Indices>
        constexpr void for_each_tuple_element(Tuple &&tuple, Mapper &&mapper, std::index_sequence<Indices ...> &&)
        {
            (mapper(std::get<Indices>(tuple)), ...);
        }
    }

    template<typename Tuple, typename Mapper>
    constexpr auto tuple_map(Tuple &&tuple, Mapper &&mapper)
    {
        return detail::tuple_map(
                std::forward<Tuple>(tuple),
                std::forward<Mapper>(mapper),
                std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
        );
    }

    /**
     * Applies a callable object on each element of a tuple-like container.
     *
     * @param tuple     The type tuple to map the function onto
     * @param mapper    The function used as a mapper
     */
    template<typename Tuple, typename Mapper>
    constexpr void for_each_tuple_element(Tuple &&tuple, Mapper &&mapper)
    {
        detail::for_each_tuple_element(
                std::forward<Tuple>(tuple),
                std::forward<Mapper>(mapper),
                std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
        );
    }
}
#endif //META_TUPLE_MAP_HPP