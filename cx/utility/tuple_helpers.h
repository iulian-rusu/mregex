#ifndef CX_TUPLE_HELPERS_H
#define CX_TUPLE_HELPERS_H

namespace cx
{
    template<typename Tuple, typename Mapper, std::size_t ... Indices>
    constexpr decltype(auto) map_tuple_impl(Tuple &&tuple, Mapper &&mapper, std::index_sequence<Indices ...> &&)
    {
        return (mapper(std::get<Indices>(tuple)), ...);
    }

    /**
     * Calls a Mapper for every element of Tuple
     *
     * @param tuple     The type tuple to map the function onto
     * @param mapper    The function used as a mapper
     * @return          The last result returned by applying mapper on any tuple element
     */
    template<typename Tuple, typename Mapper>
    constexpr decltype(auto) map_tuple(Tuple &&tuple, Mapper &&mapper)
    {
        return map_tuple_impl(
                std::forward<Tuple>(tuple),
                std::forward<Mapper>(mapper),
                std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
        );
    }
}
#endif //CX_TUPLE_HELPERS_H
