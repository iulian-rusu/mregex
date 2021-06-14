#ifndef CX_TYPE_HELPERS_H
#define CX_TYPE_HELPERS_H

/**
 * File with concepts and metafunctions for type management
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
        s.substr(index, index);
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
}

#endif //CX_TYPE_HELPERS_H
