#ifndef MREGEX_XPR_TEMPLATES_HPP
#define MREGEX_XPR_TEMPLATES_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::xpr
{
    /**
     * Adapter metafunctions for partially evaluating some AST nodes.
     */

    template<std::size_t ID, typename Name>
    struct capture_template
    {
        template<typename Inner>
        using type = ast::capture<ID, Name, Inner>;
    };

    template<match_mode Mode, typename A, typename B>
    struct repetition_template
    {
        template<typename Inner>
        using type = ast::basic_repetition<Mode, A, B, Inner>;
    };
}
#endif //MREGEX_XPR_TEMPLATES_HPP