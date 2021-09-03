# Meta-Regex
[![Build Status](https://app.travis-ci.com/iulian-rusu/meta-regex.svg?token=yYkpQVp4y1XUXWqsFXsK&branch=master)](https://app.travis-ci.com/iulian-rusu/meta-regex)
## Summary
This is a compile-time implementation of a regular expression library in 
C++20. It parses a regex pattern and compiles it into a native C++ type 
using template metaprogramming.

## Features
The library currently supports the following features:
* `constexpr` matching on any sequence that provides 
an iterator compatible with `std::forward_iterator`
* compile-time syntax checking
* various flags like `icase`, `dotall` and `multiline`
* structured binding decomposition of `meta::regex_result`
* standard regex syntax:
    * `*` - Kleene begin quantifier
    * `+` - plus quantifier
    * `?` - optional quantifier
    * `{5}` - exact quantifier
    * `{3,6}`, `{5,}` - range quantifiers
    * `(expr)` - capturing subexpressions
    * `(?:expr)` - non-capturing subexpressions
    * `(?=expr)`, `(?!expr)` - lookaheads
    * `\12` - backreferences
    * `|` - alternation of two subexpressions
    * `\?`, `\\`, `\n` - escaped characters
    * `\w`, `\d`, `\S` - special character classes
    * `.` - wildcard character
    * `^`, `$` - anchors
    * `[abc]`, `[^abc]`, `[a-z0-9]` - sets
    
## Usage
Below is an example of parsing a date.
Notice that the generated regular expression is a type and not an object, since the library maps every
valid regex pattern to a C++ type at compile-time. Invalid regular expression syntax will result
in a compilation error.

The input can be any object that satisfies the `meta::string_range` concept, or a pair of iterators that
satisfy `std::forward_iterator`.
```cpp
using date_regex = meta::regex<R"((\d{1,2})/(\d{1,2})/(\d{2,4}))">;
constexpr std::string_view date = "07/08/2021";
auto [day, month, year] = date_regex::match(date);
```

The library supports searching for multiple matches in a string. In this case,
the `meta::regex::range` method returns a generator that will lazily evaluate
all matches in the string. We can iterate through the generator
just like any range.
```cpp
using word_regex = meta::regex<R"(\w+(?=\W))">;
std::string words = "Find all word-sequences in this string!";
for (auto &&res : word_regex::range(words))
{
    std::cout << res << '\n';
}
```