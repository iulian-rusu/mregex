#include <mregex.hpp>

namespace meta::tests
{
    using zero = symbol::quantifier_value<0>;
    using one = symbol::quantifier_value<1>;
    using two = symbol::quantifier_value<2>;

    static_assert(symbol::is_quantifier<zero>);
    static_assert(symbol::is_quantifier<symbol::infinity>);
    static_assert(symbol::is_quantifier<symbol::quantifier_begin> == false);

    static_assert(symbol::is_zero<zero>);
    static_assert(symbol::is_zero<one> == false);
    static_assert(symbol::is_zero<symbol::infinity> == false);

    static_assert(symbol::is_infinity<symbol::infinity>);
    static_assert(symbol::is_infinity<zero> == false);
    static_assert(symbol::is_infinity<one> == false);

    static_assert(symbol::get_value<zero> == 0);
    static_assert(symbol::get_value<one> == 1);
    static_assert(symbol::get_value<two> == 2);

    static_assert(symbol::equals<one>(1));
    static_assert(symbol::equals<one>(2) == false);
    static_assert(symbol::equals<symbol::infinity>(0) == false);

    static_assert(std::is_same_v<symbol::decrement_t<two>, one>);
    static_assert(std::is_same_v<symbol::decrement_t<one>, zero>);
    static_assert(std::is_same_v<symbol::decrement_t<symbol::infinity>, symbol::infinity>);

    static_assert(std::is_same_v<symbol::subtract_t<two, two>, zero>);
    static_assert(std::is_same_v<symbol::subtract_t<two, one>, one>);
    static_assert(std::is_same_v<symbol::subtract_t<symbol::infinity , two>, symbol::infinity>);

    static_assert(std::is_same_v<symbol::make_name<>, symbol::name<"">>);
    static_assert(std::is_same_v<symbol::make_name<'a'>, symbol::name<"a">>);
    static_assert(std::is_same_v<symbol::make_name<'a', 'b'>, symbol::name<"ab">>);
}
