#ifndef CX_CAPTURE_COUNTER_H
#define CX_CAPTURE_COUNTER_H

namespace cx
{
    /**
     * Metafunction used to count the number of capturing groups in the regex AST
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename ... Rest>
    struct capture_counter
    {
        static constexpr std::size_t count = First::capture_count + capture_counter<Rest ...>::count;
    };

    template<typename First>
    struct capture_counter<First>
    {
        static constexpr std::size_t count = First::capture_count;
    };

    /**
     * Metafunction used to get the maximum number of capturing groups in any AST node
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename ... Rest>
    struct max_capture_counter
    {
        static constexpr std::size_t first_count = First::capture_count;
        static constexpr std::size_t rest_count = max_capture_counter<Rest ...>::count;

        static constexpr std::size_t count = first_count > rest_count ? first_count : rest_count;
    };

    template<typename First>
    struct max_capture_counter<First>
    {
        static constexpr std::size_t count = First::capture_count;
    };
}
#endif //CX_CAPTURE_COUNTER_H
