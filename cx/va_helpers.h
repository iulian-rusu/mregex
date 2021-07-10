#ifndef CX_VA_HELPERS_H
#define CX_VA_HELPERS_H

/**
 * File with metafunctions for variadic template processing
 */
namespace cx
{
    /**
     * Metafunction returning the first type in a variadic type pack
     *
     * @tparam First    The type to be returned
     */
    template<typename First, typename ...>
    struct first
    {
        using type = First;
    };

    template<typename ... Elems>
    using first_t = typename first<Elems ...>::type;

    /**
     * Metafunction returning the last type in a variadic type pack
     *
     * @tparam First    The first type in the pack
     * @tparam Rest      The rest of the type pack
     */
    template<typename First, typename ... Rest>
    struct last
    {
        using type = typename last<Rest ...>::type;
    };

    template<typename First>
    struct last<First>
    {
        using type = First;
    };

    template<typename ... Elems>
    using last_t = typename last<Elems ...>::type;

    /***
    * Metafunction used to detect wether a type is present inside a type pack
    *
    * @tparam Test     The searched type
    * @tparam First    The first type in the type pack
    * @tparam Rest     The rest of the type pack
    */
    template<typename Test, typename First, typename ... Rest>
    struct is_any_of
    {
        static constexpr auto value = std::is_same_v<Test, First> || is_any_of<Test, Rest ...>::value;
    };

    template<typename Test, typename First>
    struct is_any_of<Test, First>
    {
        static constexpr auto value = std::is_same_v<Test, First>;
    };

    template<typename Test, typename First, typename ... Rest>
    constexpr auto is_any_of_v = is_any_of<Test, First, Rest ...>::value;

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
#endif //CX_VA_HELPERS_H
