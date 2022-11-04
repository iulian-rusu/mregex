#include <iostream>
#include <mregex.hpp>

int main()
{
    // Every regex is mapped to a C++ type
    using my_regex = meta::regex<"ab+">;

    std::string_view str = "abbbb";
    auto match = my_regex::match(str);
    std::cout << (match ? "Matched!" : "Not matched") << '\n';

    // The result type can be named explicitly, although it is pretty verbose
    my_regex::result_type<std::string_view::iterator> res = my_regex::match(str);
    std::cout << res.matched() << '\n';
}