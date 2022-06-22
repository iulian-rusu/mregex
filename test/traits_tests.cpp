#include "tests.hpp"

#ifdef MREGEX_RUN_TRAITS_TESTS
namespace meta::tests
{
    using namespace ast;

    static_assert(is_any_of_v<int, int>);
    static_assert(is_any_of_v<int, float, int>);
    static_assert(is_any_of_v<int, int, long int, double>);
    static_assert(is_any_of_v<int, int, double, long, void, float>);
    static_assert(is_any_of_v<int, double, double, int, void, void>);
    static_assert(is_any_of_v<int, float, double, long, int, void>);
    static_assert(is_any_of_v<int, float, double, long, void> == false);
    static_assert(is_any_of_v<int, float, double, long> == false);
    static_assert(is_any_of_v<int, float, long int> == false);
    static_assert(is_any_of_v<int> == false);

    static_assert(is_trivially_matchable_v<nothing>);
    static_assert(is_trivially_matchable_v<negated<nothing>>);
    static_assert(is_trivially_matchable_v<literal<'a'>>);
    static_assert(is_trivially_matchable_v<whitespace>);
    static_assert(is_trivially_matchable_v<wildcard>);
    static_assert(is_trivially_matchable_v<range<'a', 'z'>>);
    static_assert(is_trivially_matchable_v<set<wildcard, literal<'b'>, whitespace>>);
    static_assert(is_trivially_matchable_v<sequence<wildcard>>);
    static_assert(is_trivially_matchable_v<alternation<wildcard>>);
    static_assert(is_trivially_matchable_v<disjunction<wildcard>>);
    static_assert(is_trivially_matchable_v<alternation<whitespace, literal<'a'>, literal<'b'>>>);
    static_assert(is_trivially_matchable_v<epsilon> == false);
    static_assert(is_trivially_matchable_v<beginning> == false);
    static_assert(is_trivially_matchable_v<backref<1>> == false);
    static_assert(is_trivially_matchable_v<sequence<wildcard, whitespace>> == false);
    static_assert(is_trivially_matchable_v<alternation<wildcard, beginning>> == false);
    static_assert(is_trivially_matchable_v<capture<1, wildcard>> == false);
    static_assert(is_trivially_matchable_v<star<literal<'a'>>> == false);
    static_assert(is_trivially_matchable_v<alternation<star<whitespace>, literal<'a'>, literal<'b'>>> == false);
}
#endif //MREGEX_RUN_TRAITS_TESTS