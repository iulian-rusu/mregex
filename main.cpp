#include <iostream>
#include "ctr.h"

int main()
{
    auto[count, matched] = ctr::match<"(abc)*x|hello world">("hello world");
    if (matched)
    {
        std::cout << "Success!\n";
        std::cout << "Matched " << count << " characters\n";
    }
    else
    {
        std::cout << "Fail!\n";
    }
}