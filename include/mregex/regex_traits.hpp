#ifndef MREGEX_REGEX_TRAITS_HPP
#define MREGEX_REGEX_TRAITS_HPP

#include <iterator>

namespace meta
{
    template<typename Regex>
    using regex_ast_t = typename Regex::ast_type;

    template<typename Regex>
    using regex_flags = typename Regex::flags;

    template<typename Regex, std::forward_iterator Iter>
    using regex_result_view_t = typename Regex::template result_view_type<Iter>;

    template<typename Regex>
    using regex_result_t = typename Regex::result_type;

    template<typename Regex, std::forward_iterator Iter>
    using regex_capture_view_storage_t = typename regex_result_view_t<Regex, Iter>::capture_storage_type;

    template<typename Regex>
    using regex_capture_storage_t = typename regex_result_t<Regex>::capture_storage_type;

    template<typename Regex, std::size_t ID, std::forward_iterator Iter>
    using regex_capture_view_t = std::tuple_element_t<ID, regex_capture_view_storage_t<Regex, Iter>>;

    template<typename Regex, std::size_t ID>
    using regex_capture_t = std::tuple_element_t<ID, regex_capture_storage_t<Regex>>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_tokenizer_t = typename Regex::template tokenizer_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_searcher_t = typename Regex::template searcher_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_token_range_t = typename Regex::template token_range_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_match_range_t = typename Regex::template match_range_type<Iter>;
}
#endif //MREGEX_REGEX_TRAITS_HPP