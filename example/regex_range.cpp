#include <iostream>
#include <mregex.hpp>

int main()
{
    using word_regex = meta::regex<R"(\w+?(?!\w))">;

    // The range() method returns a range that lazily generates the results one-by-one
    // Old results are discarded after advancing the iterator
    // Reading an iterator multiple times without incrementing will give the same result
    for (auto &&word : word_regex::range("Find all word-like sequences in this string!"))
        std::cout << word << '\n';
}