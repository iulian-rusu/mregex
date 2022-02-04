# mRegex - Metaprogramming-Based Regular Expressions
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
* standard regex syntax:
    * `*` - Kleene star quantifiers
    * `+` - plus quantifiers
    * `?` - optional quantifiers
    * `{5}` - exact quantifiers
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
Currently, building is supported on GCC 10 and Clang 12, but any compiler that is C++20 compliant 
will work.

To install the library, simply add the contents of the `include` directory 
to known system paths:
```shell
ln -s mregex/include/* /usr/include
```
Alternatively, if you are using a build system like `CMake`, you can update your 
target's include directories in a similar manner.

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

## Credits
This project was inspired by other compile-time regex libraries
like [Boost.Xpressive](https://www.boost.org/doc/libs/1_65_1/doc/html/xpressive.html)
and especially [CTRE](https://github.com/hanickadot/compile-time-regular-expressions), 
which served as a reference for the compile-time regex parser.
