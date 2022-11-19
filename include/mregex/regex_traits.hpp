#ifndef MREGEX_REGEX_TRAITS_HPP
#define MREGEX_REGEX_TRAITS_HPP

#include <mregex/regex.hpp>

namespace meta
{
    /**
     * Metafunction that provides a uniform interface for various properties of a regex-like object.
     * The type parameter must be an instance of `regex_interface` or `regex`.
     */
    template<typename Regex>
    struct regex_traits;

    template<static_string Pattern, typename... Flags>
    struct regex_traits<regex<Pattern, Flags ...>> : regex_traits<typename regex<Pattern, Flags ...>::base_type> {};

    template<typename AST, typename... Flags>
    struct regex_traits<regex_interface<AST, Flags ...>>
    {
        using flags = regex_flags_container<Flags ...>;
        using ast_type = AST;

        template<std::forward_iterator Iter>
        using context_type = typename regex_interface<AST, Flags ...>::template context_type<Iter>;

        template<std::forward_iterator Iter>
        using result_view_type = typename context_type<Iter>::result_view_type;

        using result_type = regex_result<ast::capture_name_spec_t<ast_type>>;

        template<std::forward_iterator Iter>
        using capture_view_storage_type = typename result_view_type<Iter>::capture_storage_type;

        using capture_storage_type = typename result_type::capture_storage_type;

        template<std::forward_iterator Iter>
        using generator_type = regex_match_generator<context_type<Iter>>;

        template<std::forward_iterator Iter>
        using range_type = input_range_adapter<generator_type<Iter>>;
    };

    template<typename Regex>
    using regex_flags = typename regex_traits<Regex>::flags;

    template<typename Regex>
    using regex_ast_t = typename regex_traits<Regex>::ast_type;

    template<typename Regex, std::forward_iterator Iter>
    using regex_context_t = typename regex_traits<Regex>::template context_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_result_view_t = typename regex_traits<Regex>::template result_view_type<Iter>;

    template<typename Regex>
    using regex_result_t = typename regex_traits<Regex>::result_type;

    template<typename Regex, std::forward_iterator Iter>
    using regex_capture_view_storage_t = typename regex_traits<Regex>::template capture_view_storage_type<Iter>;

    template<typename Regex>
    using regex_capture_storage_t = typename regex_traits<Regex>::capture_storage_type;

    template<typename Regex, std::forward_iterator Iter>
    using regex_generator_t = typename regex_traits<Regex>::template generator_type<Iter>;

    template<typename Regex, std::forward_iterator Iter>
    using regex_range_t = typename regex_traits<Regex>::template range_type<Iter>;
}
#endif //MREGEX_REGEX_TRAITS_HPP