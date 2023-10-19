#ifndef MREGEX_REGEX_TRAITS_HPP
#define MREGEX_REGEX_TRAITS_HPP

#include <iterator>
#include <mregex/ast/traits.hpp>
#include <mregex/symbols/names.hpp>

namespace meta
{
    template<typename Regex>
    using regex_ast_t = typename Regex::ast_type;

    template<typename Regex>
    using regex_flags = typename Regex::flags;

    template<typename Regex, std::forward_iterator Iter>
    using regex_tokenizer_t = typename Regex::template tokenizer_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_searcher_t = typename Regex::template searcher_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_token_range_t = typename Regex::template token_range_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_match_range_t = typename Regex::template match_range_type<Iter>;

    // Builds a capture name specification for the entire regex, including the implicit capturing group
    template<typename Regex>
    using regex_capture_name_spec_t = push_t<ast::capture_name_spec_t<regex_ast_t<Regex>>, symbol::unnamed>;
}
#endif //MREGEX_REGEX_TRAITS_HPP