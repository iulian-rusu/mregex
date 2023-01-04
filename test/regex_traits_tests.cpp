#include <mregex.hpp>

namespace meta::tests
{
    using empty_regex = regex<"">;
    using uri_regex = regex<R"(([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?)">;
    using email_regex = regex<R"((?<test>[^ @]+)@([^ @]+))">::with<flag::icase, flag::dotall>;

    using iterator_type = std::string_view::iterator;

    using uri_result_view_t = regex_result_view_t<uri_regex, iterator_type>;
    using uri_result_t = regex_result_t<uri_regex>;
    using uri_capture_view_t = regex_capture_view_t<uri_regex, 1, iterator_type>;
    using uri_generator_t = regex_generator_t<uri_regex, iterator_type>;
    using uri_range_t = regex_range_t<uri_regex, iterator_type>;

    using email_result_view_t = regex_result_view_t<email_regex, iterator_type>;
    using email_capture_view_t = regex_capture_view_t<email_regex, 1, iterator_type>;

    static_assert(regex_flags<email_regex>::dotall);
    static_assert(regex_flags<email_regex>::icase);
    static_assert(regex_flags<email_regex>::multiline == false);
    static_assert(regex_flags<uri_regex::base_type>::icase == false);

    static_assert(std::is_same_v<regex_ast_t<empty_regex>, ast::empty>);
    static_assert(std::is_same_v<regex_ast_t<uri_regex>, uri_regex::ast_type>);
    static_assert(std::is_same_v<regex_ast_t<uri_regex::base_type>, uri_regex::ast_type>);

    static_assert(std::is_same_v<uri_result_view_t, decltype(uri_regex::match(""))>);
    static_assert(std::is_same_v<email_result_view_t, decltype(email_regex::search(""))>);
    static_assert(std::is_same_v<uri_result_view_t, decltype(email_regex::match(""))> == false);
    static_assert(std::is_same_v<email_result_view_t, decltype(uri_regex::match(""))> == false);

    static_assert(std::is_same_v<uri_result_t, decltype(uri_regex::match("").as_memory_owner())>);
    static_assert(std::is_same_v<uri_result_t, decltype(uri_regex::match(""))> == false);
    static_assert(std::is_same_v<uri_result_t, decltype(email_regex::match("").as_memory_owner())> == false);
    static_assert(std::is_same_v<uri_result_t, decltype(email_regex::match(""))> == false);

    static_assert(std::is_same_v<uri_capture_view_t &&, decltype(uri_regex::match("").group<1>())>);
    static_assert(std::is_same_v<email_capture_view_t &&, decltype(uri_regex::match("").group<1>())> == false);
    static_assert(email_capture_view_t::has_name());
    static_assert(email_capture_view_t::name() == "test");
    static_assert(uri_capture_view_t::has_name() == false);
    static_assert(uri_capture_view_t::name().empty());

    static_assert(std::is_same_v<uri_generator_t, decltype(uri_regex::generator(""))>);
    static_assert(std::is_same_v<uri_generator_t, decltype(email_regex::generator(""))> == false);
    static_assert(std::is_same_v<uri_generator_t,decltype(uri_regex::generator(std::string{}))> == false);

    static_assert(std::is_same_v<uri_range_t, decltype(uri_regex::range(""))>);
    static_assert(std::is_same_v<uri_range_t, decltype(email_regex::range(""))> == false);
    static_assert(std::is_same_v<uri_range_t,decltype(uri_regex::range(std::string{}))> == false);
}