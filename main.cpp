#include <iostream>
#include "ctr.h"

int main()
{
    std::string_view input = "The number 0xFF777 is in base 16";
    auto[start, count, matched] = ctr::search<R"(0(x|X)(\h+)(h|H)?)">(input);
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