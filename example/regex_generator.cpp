#include <iostream>
#include <vector>
#include <mregex.hpp>

int main()
{
    // The regex matches words with 4 of 5 characters
    using my_regex = meta::regex<R"((?<!\w)\w{4,5}+(?!\w))">;

    std::string_view text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    std::vector<std::string_view> words(5);
    // Fills the vector with the generated matches
    // If the vector is too big, the rest is filled with empty strings
    std::generate(words.begin(), words.end(), my_regex::generator(text));
    for (auto &&word: words)
        std::cout << word << '\n';
}