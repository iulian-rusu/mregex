#include <algorithm>
#include <iostream>
#include <vector>
#include <mregex.hpp>

int main()
{
    // This regex matches words with 4 or 5 letters
    using my_regex = meta::regex<R"(\b\w{4,5}+\b)">;

    std::string_view text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    std::vector<std::string_view> words(5);
    // Fills the vector with the generated matches
    // When using contiguous iterators, captures can be implicitly converted to std::string_view
    std::generate(words.begin(), words.end(), my_regex::generator(text));
    for (auto &&word : words)
        std::cout << word << '\n';
}