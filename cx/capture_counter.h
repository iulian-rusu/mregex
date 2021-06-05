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
#endif //CX_CAPTURE_COUNTER_H
