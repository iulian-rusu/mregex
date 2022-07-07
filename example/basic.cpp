#include <iostream>
#include <mregex.hpp>

int main()
{
    // Every regex is mapped to a C++ type
    using my_regex = meta::regex<"ab+">;

    std::string_view str = "abbbb";
    auto match = my_regex::match(str);
    if (match)
        std::cout << "Matched!\n";
    else
        std::cout << "Not matched :(\n";
}