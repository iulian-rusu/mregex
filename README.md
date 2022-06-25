# mregex - Metaprogramming-Based Regular Expressions
[![Build Status](https://app.travis-ci.com/iulian-rusu/mregex.svg?token=yYkpQVp4y1XUXWqsFXsK&branch=master)](https://app.travis-ci.com/iulian-rusu/mregex)

This is a compile-time implementation of regular expressions in C++20.
The library parses a regex pattern and compiles it into a native C++
type using template metaprogramming.

## Features
The library offers the following features:
* `constexpr` matching on any sequence that provides 
an iterator compatible with `std::forward_iterator`
* compile-time syntax checking
* support for a large portion of the standard regex syntax:
  * quantifiers (`*`, `?`, `+`, `{N,M}`)
  * alternations and sets
  * capturing groups
  * backreferences
  * lookaheads
  * lookbehinds with arbitrary expressions (requires bidirectional iterators)
* the ability to modify your regex with flags:
  * `icase` - ignore case when matching
  * `dotall` - make `.` match newline characters
  * `multiline` - make `$` and `^` match beginnings/end of lines
  * `ungreedy` - make quantifiers match as few characters as possible
* a flexible API that allows exact matching, searching or iterating over multiple results
    
## Installation

The project is header-only and does not depend on any third-party libraries. 
Currently, building is supported on GCC 10 and Clang 12, but any compiler that is C++20 compliant 
will work.

To install the library, simply add the contents of the `include` directory 
to your compiler's include paths. If you are using a build system like `CMake`, update your 
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

## Syntax
Currently, the following syntax features are supported:

|      **Syntax**       |                                     **Effect**                                     |
|:---------------------:|:----------------------------------------------------------------------------------:|
|          `.`          |     match any character except `\n` and `\r`, unless the `dotall` flag is set      |
|          `^`          | match the beginning of the input (or of the line when the `multiline` flag is set) |
|          `$`          |    match the end of the input (or of the line when the `multiline` flag is set)    |
|        `[abc]`        |                           match any character in the set                           |
|        `[a-z]`        |                          match any character in the range                          |
|       `[^abc]`        |                       match any character **not** in the set                       |
|         `\d`          |                             match any digit character                              |
|         `\l`          |                             match any lowercase letter                             |
|         `\u`          |                             match any uppercase letter                             |
|         `\a`          |                                  match any letter                                  |
|         `\w`          |                 match any word character (letters, digits and `_`)                 |
|         `\x`          |                          match any hexadecimal character                           |
| `\D`, `\W`, `\U` etc. |                match any character **not** in the respective class                 |
|         `\ `          |   remove any special meaning from the next token (ex. `\.` will only match `.`)    |
|          `*`          |                             match any number of times                              |
|          `+`          |                                match at least once                                 |
|          `?`          |                              match one or zero times                               |
|  <code>&#124;</code>  |                           match the left or right option                           |
|         `{N}`         |                              match exactly `N` times                               |
|        `{N,}`         |                              match at least `N` times                              |
|        `{N,M}`        |                          match between `N` and `M` times                           |
|       `(expr)`        |                       capture the result of matching `expr`                        |
|      `(?:expr)`       |                             make a non-capturing group                             |
|         `\N`          |                  backreference to the capturing group number `N`                   |
|      `(?=expr)`       |       (positive lookahead) test if `expr` will match from the current point        |
|      `(?!expr)`       |            (negative lookahead) test the negation of positive lookahead            |
|      `(?<=expr)`      |  (positive lookbehind) test if `expr` would have matched before the current point  |
|      `(?<!expr)`      |           (negative lookbehind) test the negation of positive lookbehind           |

## Credits
This project was inspired by other compile-time regex libraries
like [Boost.Xpressive](https://www.boost.org/doc/libs/1_65_1/doc/html/xpressive.html)
and especially [CTRE](https://github.com/hanickadot/compile-time-regular-expressions), 
which served as a reference for the compile-time regex parser.
