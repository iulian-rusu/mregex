#ifndef META_MATCH_CONTEXT_BASE_HPP
#define META_MATCH_CONTEXT_BASE_HPP

#include "../utility/tuple_helpers.hpp"
#include "../regex_flags.hpp"

namespace meta
{
    template<std::forward_iterator Iter, typename AST>
    struct match_context_base
    {
        regex_capture_view_storage<AST::capture_count, Iter> captures{};

        constexpr void clear_captures() noexcept
        {
            tuple_for_each(captures, [](auto &capture) {
                capture.clear();
            });
        }
    };
}
#endif //META_MATCH_CONTEXT_BASE_HPP
