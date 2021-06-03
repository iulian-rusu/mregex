#ifndef CX_UTILITY_H
#define CX_UTILITY_H

#include <type_traits>

/**
 * File with various utility metafunctions
 */
namespace cx
{
    /**
     * Concept used to constrain the generic type accepted by matching/searching functions
     *
     * @tparam T    The type to be constrained
     */
    template<typename T>
    concept string_like = requires(T s, std::size_t index)
    {
        static_cast<char>(s[index]);
        static_cast<std::size_t>(s.length());
        s.begin();
        s.end();
        s.substr(index, index);
    };

    /**
     * Struct used as parameter for more concise function signatures
     */
    struct match_params
    {
        std::size_t from{};
        std::size_t max_chars{};
        bool negated = false;
    };

    /**
     * Meta-pair containing two types
     * @tparam First    The first type in the pair
     * @tparam Second   The second type in the pair
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
     * @tparam ...      The rest of the type pack
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
     * @tparam ...      The rest of the type pack
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

    /**
     * Metafunction used to count the number of capturing groups in the regex AST
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename ... Rest>
    struct count_captures
    {
        static constexpr std::size_t capture_count = First::capture_count + count_captures<Rest ...>::capture_count;
    };

    template<typename First>
    struct count_captures<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;
    };
}

#endif //CX_UTILITY_H
