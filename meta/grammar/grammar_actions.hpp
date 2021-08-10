#ifndef META_GRAMMAR_ACTIONS_HPP
#define META_GRAMMAR_ACTIONS_HPP

#include <type_traits>

namespace meta::grammar
{
    // Types used to mark actions imposed by grammar rules

    struct advance {};

    struct accept : std::true_type {};

    struct reject : std::false_type {};
}
#endif //META_GRAMMAR_ACTIONS_HPP