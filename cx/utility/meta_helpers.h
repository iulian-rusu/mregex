#ifndef CX_META_HELPERS_H
#define CX_META_HELPERS_H

#include "../ast/ast_decl.h"

namespace cx
{
    /**
     * Meta-pair containing two types
     */
    template<typename First, typename Second>
    struct pair
    {
        using first = First;
        using second = Second;
    };

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
    * Metafunction used to detect if a type is present inside a type pack
    */
    template<typename, typename ...>
    struct is_any_of;

    template<typename Test, typename First, typename ... Rest>
    struct is_any_of<Test, First, Rest ...>
    {
        static constexpr auto value = std::is_same_v<Test, First> || is_any_of<Test, Rest ...>::value;
    };

    template<typename Test>
    struct is_any_of<Test>
    {
        static constexpr auto value = false;
    };

    template<typename Test, typename First, typename ... Rest>
    constexpr auto is_any_of_v = is_any_of<Test, First, Rest ...>::value;

    template<typename>
    struct requires_successor_context : std::false_type {};

    template<typename Inner>
    struct requires_successor_context<star<Inner>> : std::true_type {};

    template<typename First, typename ... Rest>
    struct requires_successor_context<alternation<First, Rest ...>> : std::true_type {};
}
#endif //CX_META_HELPERS_H
