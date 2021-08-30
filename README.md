# Meta-Regex

## Summary
This is a compile-time implementation of a regular expression library in 
C++20. It parses a regex pattern and compiles it into a native C++ type 
using template metaprogramming.

## Features
The library currently supports the following features:
* `constexpr` matching using `std::forward_iterator` semantics
* compile-time syntax checking
* various flags like `ignore_case`, `dotall` and `multiline`
* structured binding decomposition of `meta::regex_result`
* standard regex syntax:
    * `*` - Kleene start quantifier
    * `+` - plus quantifier
    * `?` - optional quantifier
    * `{5}` - exact quantifier
    * `{3,6}`, `{5,}` - range quantifiers
    * `(expr)` - capturing subexpressions
    * `(?:expr)` - non-capturing subexpressions
    * `\12` - backreferences
    * `|` - alternation of two subexpressions
    * `\?`, `\\`, `\n` - escaped characters
    * `\w`, `\d`, `\S` - special character classes
    * `.` - wildcard character
    * `^`, `$` - anchors
    * `[abc]`, `[^abc]`, `[a-z0-9]` - sets
    
## Usage
Below is an example of extracting data from a URL-string.
Notice that the generated regular expression is a type and not an object, since the library maps every
input regex pattern to a C++ type at compile-time. Invalid regular expression syntax will result
in a compilation error.

The input can be any object that satisfies the `meta::string_like` concept, or a pair of iterators that
satisfy `std::forward_iterator`.
```cpp
using url_regex = meta::regex<R"((\w+):\/\/(?:(?:(\w+)?(?::(\w+))?@)?([\w.]+)(?::(\d+))?)?(?:(\/[-/\w]+)?\?([\w=&]+))?)">;

constexpr std::string_view url = "https://username:password@hostname.com:8080/path/to/resource?id=12345";
constexpr auto match_res = url_regex::match(url);
std::cout << "Scheme:\t" << match_res.group<1>() << '\n';
std::cout << "User:\t" << match_res.group<2>() << '\n';
std::cout << "Pass:\t" << match_res.group<3>() << '\n';
std::cout << "Host:\t" << match_res.group<4>() << '\n';
std::cout << "Port:\t" << match_res.group<5>() << '\n';
std::cout << "Path:\t" << match_res.group<6>() << '\n';
std::cout << "Query:\t" << match_res.group<7>() << "\n";
```

The library supports searching for multiple matches in a string. In this case,
the `meta::regex::find_all` method returns a generator that will evaluate
on-demand all matches in the string. We can iterate through the generator
just like any standard container.
```cpp
using word_regex = meta::regex<R"([-a-z']+)", meta::flag::i>;

constexpr std::string_view words = "Let's iterate over these words!";
for (auto &&res : word_regex::find_all(words))
{
    std::cout << res << '\n';
}
```

C++17 structured binding declaration is also supported.
```cpp
using date_regex = meta::regex<R"((\d+)\/(\d+)\/(\d+))">;
constexpr std::string_view date = "07/08/2021";
auto [day, month, year] = date_regex::match(date);
```
