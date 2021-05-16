#ifndef META_TUPLE_MAP_HPP
#define META_TUPLE_MAP_HPP

#include <tuple>

namespace meta
{
    template<typename Tuple, typename Mapper, std::size_t ... Indices>
    constexpr decltype(auto) tuple_map_helper(Tuple &&tuple, Mapper &&mapper, std::index_sequence<Indices ...> &&)
    {
        return (mapper(std::get<Indices>(tuple)), ...);
    }

    /**
     * Calls a Mapper for each element of a tuple-like container.
     *
     * @param tuple     The type tuple to map the function onto
     * @param mapper    The function used as a mapper
     * @return          The result returned by applying mapper on the last tuple element
     */
    template<typename Tuple, typename Mapper>
    constexpr decltype(auto) tuple_map(Tuple &&tuple, Mapper &&mapper)
    {
        return tuple_map_helper(
                std::forward<Tuple>(tuple),
                std::forward<Mapper>(mapper),
                std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
        );
    }
}
#endif //META_TUPLE_MAP_HPP