#ifndef CX_CONCEPTS_H
#define CX_CONCEPTS_H

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
}
#endif //CX_CONCEPTS_H
