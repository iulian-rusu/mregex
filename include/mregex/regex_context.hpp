#ifndef MREGEX_REGEX_CONTEXT_HPP
#define MREGEX_REGEX_CONTEXT_HPP

#include <mregex/utility/tuple.hpp>
#include <mregex/regex_flags.hpp>
#include <mregex/regex_result.hpp>

namespace meta
{
    /**
     * Data structure that holds static information about the regex as well as
     * match-time information like captured contents.
     */
    template<std::forward_iterator Iter, typename AST, typename... Flags>
    struct regex_context
    {
        static_assert((is_flag<Flags> && ...), "invalid flag");

        using flags = regex_flags_container<Flags ...>;
        using ast_type = AST;
        using iterator_type = Iter;
        using result_view_type = regex_result_view<ast::capture_name_spec_t<ast_type>, iterator_type>;
        using capture_view_storage_type = typename result_view_type::capture_storage_type;

        capture_view_storage_type captures{};

        constexpr void clear() noexcept
        {
            iterate_tuple(captures, [](auto &capture) {
                capture.clear();
            });
        }
    };

    template<typename Context>
    using context_flags = typename Context::flags;
}
#endif //MREGEX_REGEX_CONTEXT_HPP