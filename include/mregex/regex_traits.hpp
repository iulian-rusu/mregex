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
    using regex_context_t = typename Regex::template context_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_result_view_t = typename Regex::template result_view_type<Iter>;

    template<typename Regex>
    using regex_result_t = typename Regex::result_type;

    template<typename Regex, std::forward_iterator Iter>
    using regex_capture_view_storage_t = typename Regex::template capture_view_storage_type<Iter>;

    template<typename Regex>
    using regex_capture_storage_t = typename Regex::capture_storage_type;

    template<std::size_t ID, typename Regex, std::forward_iterator Iter>
    using regex_capture_view_t = typename Regex::template capture_view_type<ID, Iter>;

    template<std::size_t ID, typename Regex>
    using regex_capture_t = typename Regex::template capture_type<ID>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_generator_t = typename Regex::template generator_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_range_t = typename Regex::template range_type<Iter>;
}
#endif //MREGEX_REGEX_TRAITS_HPP