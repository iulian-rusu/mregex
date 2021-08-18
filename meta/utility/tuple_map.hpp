#ifndef META_TUPLE_MAP_HPP
#define META_TUPLE_MAP_HPP

#include <tuple>

namespace meta
{
    namespace impl
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

    /**
     * Creates a new tuple by applying a mapper function on each element of another tuple.
     *
     * @param tuple     The original tuple
     * @param mapper    The function that generates elements for the new tuple
     * @return          A new std::tuple with elements returned by mapper
     */
    template<typename Tuple, typename Mapper>
    constexpr auto tuple_map(Tuple &&tuple, Mapper &&mapper)
    {
        return impl::tuple_map(
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
        impl::for_each_tuple_element(
                std::forward<Tuple>(tuple),
                std::forward<Mapper>(mapper),
                std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
        );
    }
}
#endif //META_TUPLE_MAP_HPP