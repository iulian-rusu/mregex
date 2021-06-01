# Constexpr Regex in C++

## Summary
This is a minimalist `constexpr` implementation of a regular expression
library using template metaprogramming in C++20. The implementation relies
heavily on the declarative behavior of templates in C++ and uses 
functional techniques to implement the parser and generate the abstract syntax 
tree from an input string.

## Features
The library currently supports the following regex features:
* full support for `constexpr` matching
* detecting incorrect syntax at compile-time
* extracting captures
* `*` - Kleene start quantifier
* `+` - plus quantifier
* `?` - optional quantifier
* `(expr)` - subexpressions
* `|` - alternation of two subexpressions
* `\?`, `\*`, `\\` - escaping characters
* `\w`, `\d`, `\S` - special character classes
* `.` - wildcard character
* `^`, `$` - string beginning/ending matchers
* `[abc]` - sets
* `[^abc]` - negated sets
* `[a-z0-9]` - ranges inside sets

Some features planned for the future:
* `(?:expr)` - non-capturing groups
* `++`, `?+`, `*+` - possesive quantifiers
* `{5}` - exact quantifier
* `{3, 5}` - range quantifier

## Usage
Below is an example of extracting data from a URL-string.
Notice that the generated regular expression is a type and not an object, since the library maps every
input regex string to a C++ type at compile-time. Invalid regular expression syntax will result
in a compilation error.

The input can be any object that satisfies the `cx::string_like` concept.
```cpp
using url_regex = cx::regex<R"((\w+):\/\/(((\w+):(\w+)?@)?([\w.]+)(:(\d+))?)?(\/([-/\w]+)?\?([\w=&]+))?)">;

std::string_view sv = "https://admin:pass123@hostname.com:8080/path/to/resource?id=12345";
auto match_res = url_regex::match(sv);
std::cout << "Scheme:\t" << match_res.get<1>() << '\n';
std::cout << "User:\t" << match_res.get<4>() << '\n';
std::cout << "Pass:\t" << match_res.get<5>() << '\n';
std::cout << "Host:\t" << match_res.get<6>() << '\n';
std::cout << "Port:\t" << match_res.get<8>() << '\n';
std::cout << "Path:\t" << match_res.get<10>() << '\n';
std::cout << "Query:\t" << match_res.get<11>() << '\n';
```

Searching and matching can also be done in `constexpr` if the input is known at 
compile-time. Using `if constexpr` we can conditionally compile code based
on regex matching results. For example, we can generate functions with a different
return type based on some format pattern (`double` or `std::string`).
```cpp
template<cx::static_string const input>
constexpr auto parse()
{
    using test_number = cx::regex<R"([1-9]+(\.\d*)?(e(\+|-)?\d+(\.\d*)?)?)">;
    
    constexpr std::string_view sv(input.buffer, input.length());
    if constexpr (test_number::match(sv))
    {
        return std::stod(std::string(sv));
    }
    else
    {
        return std::string(sv);
    }
}
```