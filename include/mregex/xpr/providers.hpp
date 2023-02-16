#ifndef MREGEX_XPR_PROVIDERS_HPP
#define MREGEX_XPR_PROVIDERS_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::xpr
{
    /**
     * Provider metafunctions used to partially evaluate the declaration of AST nodes.
     */

    template<std::size_t ID, typename Name>
    struct capture_provider
    {
        template<typename Inner>
        using type = ast::capture<ID, Name, Inner>;
    };

    template<match_mode Mode, symbol::finite_quantifier A, symbol::quantifier B>
    struct repetition_provider
    {
        template<typename Inner>
        using type = ast::basic_repetition<Mode, A, B, Inner>;
    };
}
#endif //MREGEX_XPR_PROVIDERS_HPP