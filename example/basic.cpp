#include <iostream>
#include <mregex.hpp>

int main()
{
    // Every regex is mapped to a C++ type
    using my_regex = meta::regex<"ab+">;

    auto match = my_regex::match("abbbb");
    std::cout << (match ? "Matched!" : "Not matched") << '\n';
}