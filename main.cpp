#include <iostream>
#include "cxregex.h"

int main()
{
    std::string_view input = "The number 0xFF777 is in base 16";
    auto[start, count, matched] = cx::regex<R"(0(x|X)(\h+)(h|H)?)">::search(input);
    if (matched)
    {
        std::cout << "Matched " << count << " characters: ";
        std::cout << input.substr(start, count) << '\n';
    }
    else
    {
        std::cout << "Fail!\n";
    }
}