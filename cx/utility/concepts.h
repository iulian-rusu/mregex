#ifndef CX_CONCEPTS_H
#define CX_CONCEPTS_H

#include "meta_helpers.h"

namespace cx
{
    /**
     * Concept used to constrain the generic type accepted by matching/searching functions
     */
    template<typename T>
    concept string_like = requires(T s, std::size_t index)
    {
        static_cast<char>(s[index]);
        static_cast<std::size_t>(s.length());
        s.substr(index, index);
    };

    /**
     * Concept used to constrain a type used as a value producer for cx::generator
     */
    template<typename T>
    concept producer = requires(T p)
    {
        p();
    };

    /**
     * Concept used to constrain the inner node of cx::negated
     */
    template<typename T>
    concept negatable = is_terminal_v<T> || is_range_v<T> || is_alternation_v<T>;
}
#endif //CX_CONCEPTS_H
