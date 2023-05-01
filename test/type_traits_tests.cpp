#include <mregex.hpp>

namespace meta::tests
{
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

    static_assert(std::is_same_v<remove_rvalue_cvref_t<std::string>, std::string>);
    static_assert(std::is_same_v<remove_rvalue_cvref_t<std::string const>, std::string>);
    static_assert(std::is_same_v<remove_rvalue_cvref_t<std::string &&>, std::string>);
    static_assert(std::is_same_v<remove_rvalue_cvref_t<std::string const &&>, std::string>);
    static_assert(std::is_same_v<remove_rvalue_cvref_t<std::string &>, std::string &>);
    static_assert(std::is_same_v<remove_rvalue_cvref_t<std::string const &>, std::string const &>);
}