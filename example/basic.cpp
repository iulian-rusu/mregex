#include <iostream>
#include <mregex.hpp>

int main()
{
    // Every regex is mapped to a C++ type
    using my_regex = meta::regex<"ab+">;

    std::string_view str = "abbbb";
    // The match result is contextually convertible to bool
    if (auto match = my_regex::match(str))
        std::cout << "Matched!\n";
    else
        std::cout << "Not matched :(\n";
}