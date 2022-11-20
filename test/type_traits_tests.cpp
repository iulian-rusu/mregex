#include <mregex.hpp>

namespace meta::tests
{
    static_assert(is_any_of<int, int>);
    static_assert(is_any_of<int, float, int>);
    static_assert(is_any_of<int, int, long int, double>);
    static_assert(is_any_of<int, int, double, long, void, float>);
    static_assert(is_any_of<int, double, double, int, void, void>);
    static_assert(is_any_of<int, float, double, long, int, void>);
    static_assert(is_any_of<int, float, double, long, void> == false);
    static_assert(is_any_of<int, float, double, long> == false);
    static_assert(is_any_of<int, float, long int> == false);
    static_assert(is_any_of<int> == false);

    static_assert(is_expiring_memory_owner<std::string> == false);
    static_assert(is_expiring_memory_owner<std::string const> == false);
    static_assert(is_expiring_memory_owner<std::string &> == false);
    static_assert(is_expiring_memory_owner<std::string const &> == false);
    static_assert(is_expiring_memory_owner<std::string &&>);
    static_assert(is_expiring_memory_owner<std::string const &&>);
    static_assert(is_expiring_memory_owner<std::string_view> == false);
    static_assert(is_expiring_memory_owner<std::string_view const> == false);
    static_assert(is_expiring_memory_owner<std::string_view &> == false);
    static_assert(is_expiring_memory_owner<std::string_view const &> == false);
    static_assert(is_expiring_memory_owner<std::string_view &&> == false);
    static_assert(is_expiring_memory_owner<std::string_view const &&> == false);

    static_assert(std::is_same_v<forward_result_t<std::string>, std::string>);
    static_assert(std::is_same_v<forward_result_t<std::string &&>, std::string>);
    static_assert(std::is_same_v<forward_result_t<std::string &>, std::string &>);

    static_assert(std::is_same_v<make_name<>, symbol::name<"">>);
    static_assert(std::is_same_v<make_name<'a'>, symbol::name<"a">>);
    static_assert(std::is_same_v<make_name<'a', 'b'>, symbol::name<"ab">>);
    static_assert(std::is_same_v<make_name<'a', 'b', 'c'>, symbol::name<"abc">>);
}