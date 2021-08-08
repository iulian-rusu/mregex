#ifndef CX_ATOMIC_COUNTER_H
#define CX_ATOMIC_COUNTER_H

namespace cx
{
    /**
     * Metafunction used to count the number of atomic groups in the regex AST.
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename ... Rest>
    struct atomic_counter
    {
        static constexpr std::size_t count = First::atomic_count + atomic_counter<Rest ...>::count;
    };

    template<typename First>
    struct atomic_counter<First>
    {
        static constexpr std::size_t count = First::atomic_count;
    };

    /**
     * Metafunction used to get the maximum number of atomic groups in any AST node.
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename ... Rest>
    struct max_atomic_counter
    {
        static constexpr std::size_t first_count = First::atomic_count;
        static constexpr std::size_t rest_count = max_atomic_counter<Rest ...>::count;
        static constexpr std::size_t count = first_count > rest_count ? first_count : rest_count;
    };

    template<typename First>
    struct max_atomic_counter<First>
    {
        static constexpr std::size_t count = First::atomic_count;
    };
}
#endif //CX_ATOMIC_COUNTER_H
