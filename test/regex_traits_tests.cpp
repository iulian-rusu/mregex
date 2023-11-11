#include <mregex.hpp>

namespace meta::tests
{
    using symbol::name;
    using symbol::unnamed;

    using empty_regex = regex<"">;
    using uri_regex = regex<R"(([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?)">;
    using email_regex = regex<R"((?<test>[^ @]+)@([^ @]+))">;

    using iterator = std::string_view::iterator;

    using uri_tokenizer_t = regex_tokenizer_t<uri_regex, iterator>;
    using uri_searcher_t = regex_searcher_t<uri_regex, iterator>;
    using uri_token_range_t = regex_token_range_t<uri_regex, iterator>;
    using uri_match_range_t = regex_match_range_t<uri_regex, iterator>;

    using empty_regex_with_flags = empty_regex::add_flags<regex_flag::icase, regex_flag::dotall>;
    static_assert(std::is_same_v<regex<"", regex_flag::icase, regex_flag::dotall>, empty_regex_with_flags>);
    static_assert(regex_flags<empty_regex_with_flags>::dotall);
    static_assert(regex_flags<empty_regex_with_flags>::icase);
    static_assert(regex_flags<empty_regex_with_flags>::multiline == false);
    static_assert(regex_flags<empty_regex_with_flags>::ungreedy == false);

    static_assert(std::is_same_v<regex_ast_t<empty_regex>, ast::empty>);
    static_assert(std::is_same_v<regex_ast_t<uri_regex>, uri_regex::ast_type>);

    static_assert(std::is_same_v<regex_capture_name_spec_t<empty_regex>, type_sequence<unnamed>>);
    static_assert(std::is_same_v<regex_capture_name_spec_t<uri_regex>, type_sequence<unnamed, unnamed, unnamed, unnamed>>);
    static_assert(std::is_same_v<regex_capture_name_spec_t<email_regex>, type_sequence<unnamed, name<"test">, unnamed>>);

    static_assert(std::is_same_v<uri_tokenizer_t, decltype(uri_regex::tokenizer(""))>);
    static_assert(std::is_same_v<uri_tokenizer_t, decltype(email_regex::tokenizer(""))> == false);
    static_assert(std::is_same_v<uri_tokenizer_t, decltype(uri_regex::tokenizer(std::string{}))> == false);
    static_assert(std::is_same_v<uri_tokenizer_t, decltype(uri_regex::searcher(""))> == false);

    static_assert(std::is_same_v<uri_searcher_t, decltype(uri_regex::searcher(""))>);
    static_assert(std::is_same_v<uri_searcher_t, decltype(email_regex::searcher(""))> == false);
    static_assert(std::is_same_v<uri_searcher_t, decltype(uri_regex::searcher(std::string{}))> == false);
    static_assert(std::is_same_v<uri_searcher_t, decltype(uri_regex::tokenizer(""))> == false);

    static_assert(std::is_same_v<uri_token_range_t, decltype(uri_regex::tokenize(""))>);
    static_assert(std::is_same_v<uri_token_range_t, decltype(email_regex::tokenize(""))> == false);
    static_assert(std::is_same_v<uri_token_range_t, decltype(uri_regex::tokenize(std::string{}))> == false);
    static_assert(std::is_same_v<uri_token_range_t, decltype(uri_regex::find_all(""))> == false);

    static_assert(std::is_same_v<uri_match_range_t, decltype(uri_regex::find_all(""))>);
    static_assert(std::is_same_v<uri_match_range_t, decltype(email_regex::find_all(""))> == false);
    static_assert(std::is_same_v<uri_match_range_t, decltype(uri_regex::find_all(std::string{}))> == false);
    static_assert(std::is_same_v<uri_match_range_t, decltype(uri_regex::tokenize(""))> == false);
}