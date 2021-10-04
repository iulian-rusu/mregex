# mregex - Metaprogramming-Based Regular Expressions
[![Build Status](https://app.travis-ci.com/iulian-rusu/mregex.svg?token=yYkpQVp4y1XUXWqsFXsK&branch=master)](https://app.travis-ci.com/iulian-rusu/mregex)

This is a compile-time implementation of regular expressions in C++20.
The library parses a regex pattern and compiles it into a native C++
type using template metaprogramming.

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
    
## Installation

The project is header-only and does not depend on any third-party libraries. 
Currently, building is supported on GCC 10 and Clang 12.
Earlier compiler versions do not fully implement some C++20 features like CNTTP and
are not tested.

To use the library in your CMake project, update your include directories in
`CMakeList.txt` with the path to `include`. 
Also make sure the CMake project is using at least C++20.

If you don't want to use CMake, you can simply add the project's `include`
directory to the header search path. Assuming you cloned the repository to `/PATH/TO/REPO/`, you can compile the demo like this:
```
$ c++ -std=c++20 -O3 -I/PATH/TO/REPO/mregex/include -o demo demo.cpp
```

## Usage
Below is an example of parsing a date.
Notice that the generated regular expression is a type and not an object, since the library maps every
valid regex pattern to a C++ type at compile-time. Invalid regular expression syntax will result
in a compilation error.

The input can be any object that satisfies the `meta::char_range` concept, or a pair of iterators that
satisfy `std::forward_iterator`.
```cpp
using date_regex = meta::regex<R"((\d{1,2})/(\d{1,2})/(\d{2,4}))">;
constexpr std::string_view date = "07/08/2021";
auto [day, month, year] = date_regex::match(date);
```

The library supports searching for multiple matches in a string. In this case,
the `meta::regex::range` method returns a type that satisfies `std::ranges::input_range`
and can be iterated to lazily generate all matches in the string. Since the range only provides 
input iterators, all results are discarded and cannot be visited again after the first
iteration.
```cpp
using word_regex = meta::regex<R"(\w+(?=\W))">;
std::string words = "Find all word-like sequences in this string!";
for (auto &&res : word_regex::range(words))
{
    std::cout << res << '\n';
}
```
