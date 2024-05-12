#include <mregex.hpp>
#include <string>

namespace meta::tests
{
    namespace
    {
        template<static_string String>
        constexpr auto string_equals(std::string_view expected) noexcept
        {
            return static_cast<std::string_view>(String) == expected;
        }

        template<static_string String>
        constexpr auto range_equals(std::initializer_list<char> expected) noexcept
        {
            return std::equal(String.begin(), String.end(), expected.begin(), expected.end());
        }
    }

    static_assert(static_string{""}.length() == 0);
    static_assert(static_string{"a"}.length() == 1);
    static_assert(static_string{"abc"}.length() == 3);
    static_assert(static_string{"abc\0"}.length() == 4);
    static_assert(static_string{"abc\0d"}.length() == 5);

    static_assert(string_equals<"">(""));
    static_assert(string_equals<"a">("a"));
    static_assert(string_equals<"abc">("abc"));
    static_assert(string_equals<"abc\0">({"abc\0", 4}));
    static_assert(string_equals<"abc\0d">({"abc\0d", 5}));

    static_assert(range_equals<"">({}));
    static_assert(range_equals<"a">({'a'}));
    static_assert(range_equals<"abc">({'a', 'b', 'c'}));
    static_assert(range_equals<"abc\0">({'a', 'b', 'c', '\0'}));
    static_assert(range_equals<"abc\0d">({'a', 'b', 'c', '\0', 'd'}));

    static_assert(range_equals<make_static_string()>({}));
    static_assert(range_equals<make_static_string<'a'>()>({'a'}));
    static_assert(range_equals<make_static_string<'a', 'b', 'c'>()>({'a', 'b', 'c'}));
    static_assert(range_equals<make_static_string<'a', 'b', 'c', '\0'>()>({'a', 'b', 'c', '\0'}));
    static_assert(range_equals<make_static_string<'a', 'b', 'c', '\0', 'd'>()>({'a', 'b', 'c', '\0', 'd'}));
}