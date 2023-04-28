# mregex - Metaprogramming-Based Regular Expressions
[![build](https://github.com/iulian-rusu/mregex/actions/workflows/build.yml/badge.svg)](https://github.com/iulian-rusu/mregex/actions/workflows/build.yml)

This is a compile-time implementation of regular expressions in C++20.
The library parses a regex pattern and compiles it into a native C++ type using template metaprogramming.

## Features
The library offers the following features:
* `constexpr` matching on any sequence that provides 
an iterator compatible with `std::forward_iterator`
* compile-time syntax checking
* support for a large portion of the standard regex syntax:
  * quantifiers (`*`, `?`, `+`, `{N,M}`), including lazy and possessive versions
  * alternations and sets
  * capturing groups, including named captures
  * backreferences
  * lookaheads
  * lookbehinds with arbitrary expressions (requires bidirectional iterators)
* flags that modify the matching behaviour:
  * `icase` - enables case-insensitive matching
  * `multiline` - enables multi-line mode, in which the anchors `^`/`$` will also match line boundaries
  * `ungreedy` - swaps lazy and greedy quantifiers
  * `dotall` - allows the wildcard `.` to also match `\n` and `\r`
* a flexible API that allows exact matching, searching, tokenizing or iterating over multiple matches
* ability to define the regex using a standard string-based API or using an [expression-based API](example/using_expressions.cpp)
    
## Installation
The project is header-only and does not depend on any third-party libraries. 
Currently, building is supported on GCC 10 and Clang 12, but any compiler that is C++20 compliant will work.

To install the library, simply add the contents of the `include` directory to your compiler's include paths.
If you are using a build system like `CMake`, update your target's include directories or add a library dependency to `mregex`.

## Usage
Below is an example of parsing a date.
Notice that the generated regular expression is a type and not an object, since the library maps every valid regex pattern to a C++ type at compile-time. 
Invalid regular expression syntax will result in a compilation error.

The input can be any object that satisfies the `meta::char_range` concept, or a pair of iterators that satisfy `std::forward_iterator`.
```cpp
using date_regex = meta::regex<R"((\d{1,2})/(\d{1,2})/(\d{2,4}))">;
constexpr std::string_view date = "07/08/2021";
auto [day, month, year] = date_regex::match(date);
```
Each regex type offers the following API:
```cpp
// Perform an exact match over the entire range
template<std::forward_iterator Iter>
static constexpr auto match(Iter begin, Iter end) -> result_view_type<Iter>;

// Match the prefix of the range
template<std::forward_iterator Iter>
static constexpr auto match_prefix(Iter begin, Iter end) -> result_view_type<Iter>;

// Search the first match in the range
template<std::forward_iterator Iter>
static constexpr auto search(Iter begin, Iter end) -> result_view_type<Iter>;

// Get a generator that lazily computes matches until the first non-match position
template<std::forward_iterator Iter>
static constexpr auto tokenizer(Iter begin, Iter end) -> tokenizer_type<Iter>;

// Get a generator that lazily searches all matches in the range, skipping non-match positions
template<std::forward_iterator Iter>
static constexpr auto searcher(Iter begin, Iter end) -> searcher_type<Iter>;

// Get a lazy view of all tokens in the range
template<std::forward_iterator Iter>
static constexpr auto tokenize(Iter begin, Iter end) -> token_range_type<Iter>;

// Get a lazy view of all matches in the range
template<std::forward_iterator Iter>
static constexpr auto find_all(Iter begin, Iter end) -> match_range_type<Iter>;
```

The [expression-based API](example/using_expressions.cpp) allows defining a regex as a composition of C++ expressions 
(similar to [Boost.Xpressive](https://www.boost.org/doc/libs/1_65_1/doc/html/xpressive.html)). 
For example, the regex `\w+(?!\w)` can be defined as follows:
```cpp
namespace xpr = meta::xpr;
using namespace xpr::operators;

auto word_regex = +xpr::word >> not xpr::ahead(xpr::word);
std::string words = "Find all word-like sequences in this string!";
for (auto &&word : word_regex.find_all(words))
    std::cout << word << '\n';
```

The main advantage of this approach is the ability to construct complex expressions from simpler components:
```cpp
// Constructing a regex equivalent to '(?:ftp|ssh|https?)://(?<domain>:[-.a-z])+.com(?:/\S*)?'
auto schema = xpr::str<"ftp"> | xpr::str<"ssh"> | xpr::regex<"https?">;
auto domain = +xpr::regex<"[-.a-z]"> >> xpr::str<".com">;
auto path = xpr::chr<'/'> >> *!xpr::whitespace;
auto url = schema >> xpr::str<"://"> >> xpr::capture<1, "domain">(domain) >> xpr::maybe(path);
```

More examples can be found in the `example/` directory.

## Syntax
Below is a complete list of the supported syntax constructs:

|      **Syntax**       |                                     **Effect**                                     |
|:---------------------:|:----------------------------------------------------------------------------------:|
|          `.`          |     match any character except `\n` and `\r`, unless the `dotall` flag is set      |
|          `^`          | match the beginning of the input (or of the line when the `multiline` flag is set) |
|          `$`          |    match the end of the input (or of the line when the `multiline` flag is set)    |
|         `\A`          |                     match strictly the beginning of the input                      |
|         `\Z`          |       match strictly the end of the input (does not allow trailing newlines)       |
|         `\b`          |     (word boundary) match the position between a word and a non-word character     |
|         `\B`          |                    match everything that is not a word boundary                    |
|         `\d`          |                             match any digit character                              |
|         `\w`          |                 match any word character (letters, digits and `_`)                 |
|         `\s`          |                           match any whitespace charater                            |
| `\D`, `\W`, `\S` etc. |                match any character **not** in the respective class                 |
|  `\x1f`, `\xA9` etc.  |                 match a character specified by its ASCII hex code                  |
|        `[abc]`        |                           match any character in the set                           |
|        `[a-z]`        |                    match any character in the ASCII code range                     |
|       `[^abc]`        |                       match any character **not** in the set                       |
|        `[\b]`         |                         match a `\b` (backspace) character                         |
|  <code>&#124;</code>  |                         match the left or right expression                         |
|          `*`          |                             match any number of times                              |
|          `+`          |                                match at least once                                 |
|          `?`          |                              match one or zero times                               |
|         `{N}`         |                              match exactly `N` times                               |
|        `{N,}`         |                              match at least `N` times                              |
|        `{N,M}`        |                          match between `N` and `M` times                           |
| `??`, `*?`, `{N,M}?`  |                    match as few characters as possible (lazily)                    |
| `?+`, `*+`, `{N,M}+`  |                     match without backtracking (possessively)                      |
|       `(expr)`        |                       capture the result of matching `expr`                        |
|         `\N`          |                  backreference to the capturing group number `N`                   |
|    `(?<name>expr)`    |                   capture by name the result of matching `expr`                    |
|      `\k<name>`       |                      backreference to a named capturing group                      |
|      `(?:expr)`       |                             make a non-capturing group                             |
|      `(?=expr)`       |       (positive lookahead) test if `expr` will match from the current point        |
|      `(?!expr)`       |            (negative lookahead) test the opposite of positive lookahead            |
|      `(?<=expr)`      |  (positive lookbehind) test if `expr` would have matched before the current point  |
|      `(?<!expr)`      |           (negative lookbehind) test the opposite of positive lookbehind           |

## Credits
This project was inspired by other compile-time regex libraries like [Boost.Xpressive](https://www.boost.org/doc/libs/1_65_1/doc/html/xpressive.html)
and especially [CTRE](https://github.com/hanickadot/compile-time-regular-expressions), which served as a reference for the compile-time regex parser.
