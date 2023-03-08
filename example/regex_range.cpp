#include <iostream>
#include <mregex.hpp>

int main()
{
    using word_regex = meta::regex<R"(\w+?(?!\w))">;

    // The find_all() method returns an input range that lazily generates matches
    std::cout << "With find_all():\n";
    for (auto &&word : word_regex::find_all("Find all word-like sequences in this string!"))
        std::cout << word << '\n';

    // Tokenizing stops at the first non-match position (in this case, after matching "Find")
    std::cout << "\nWith tokenize():\n";
    for (auto &&token : word_regex::tokenize("Find all word-like sequences in this string!"))
        std::cout << token << '\n';
}