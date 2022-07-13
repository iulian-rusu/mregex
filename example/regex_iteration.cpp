#include <iostream>
#include <mregex.hpp>

int main()
{
    // The ungreedy flag can be used to enable lazy quantifiers (*, +, {N,M})
    using word_regex = meta::regex<R"(\w+(?=\W))", meta::flag::ungreedy>;

    // The range() method returns a range that lazily generates the results one-by-one
    // Old results are discarded after advancing the iterator
    // Reading an iterator multiple times without incrementing will give the same result
    for (auto &&res : word_regex::range("Find all word-like sequences in this string!"))
        std::cout << res << '\n';
}