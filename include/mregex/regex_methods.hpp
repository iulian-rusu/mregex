#ifndef MREGEX_REGEX_METHODS_HPP
#define MREGEX_REGEX_METHODS_HPP

#include <mregex/ast/ast.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/regex_capture.hpp>

namespace meta
{
    /**
     * Method used for exact matching of a range.
     */
    template<typename Regex>
    struct regex_match_method
    {
        using regex_type = Regex;
        using ast_type = regex_ast_t<regex_type>;

        template<std::forward_iterator Iter, typename Context>
        static constexpr auto invoke(Iter begin, Iter end, Iter current, Context &ctx) noexcept -> ast::match_result<Iter>
        {
            auto result = ast_type::match(begin, end, current, ctx, continuations<Iter>::equals(end));
            std::get<0>(ctx.captures) = regex_capture_view<Iter>{current, end};
            return result;
        }
    };

    /**
     * Method used for matching the prefix of a range.
     */
    template<typename Regex>
    struct regex_match_prefix_method
    {
        using regex_type = Regex;
        using ast_type = regex_ast_t<regex_type>;

        template<std::forward_iterator Iter, typename Context>
        static constexpr auto invoke(Iter begin, Iter end, Iter current, Context &ctx) noexcept -> ast::match_result<Iter>
        {
            auto result = ast_type::match(begin, end, current, ctx, continuations<Iter>::success);
            std::get<0>(ctx.captures) = regex_capture_view<Iter>{current, result.end};
            return result;
        }
    };

    /**
     * Method used for searching the first match in a range.
     */
    template<typename Regex>
    struct regex_search_method
    {
        using regex_type = Regex;
        using ast_type = regex_ast_t<regex_type>;

        template<std::forward_iterator Iter, typename Context>
        static constexpr auto invoke(Iter begin, Iter end, Iter current, Context &ctx) noexcept -> ast::match_result<Iter>
        {
            for (;; ++current)
            {
                if (auto result = ast_type::match(begin, end, current, ctx, continuations<Iter>::success))
                {
                    std::get<0>(ctx.captures) = regex_capture_view<Iter>{current, result.end};
                    return result;
                }
                if (current == end)
                    break;
            }
            return ast::non_match(current);
        }
    };
}
#endif //MREGEX_REGEX_METHODS_HPP