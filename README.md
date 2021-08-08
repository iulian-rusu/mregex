# Constexpr Regex in C++

## Summary
This is a minimalist `constexpr` implementation of a regular expression
library using template metaprogramming in C++20. The implementation relies
heavily on the declarative behavior of templates in C++ and uses
functional techniques to implement the parser and generate the abstract syntax
tree from an input string.

## Features
The library currently supports the following regex features:
* `constexpr` matching
* compile-time syntax checking
* `*` - Kleene start quantifier
* `+` - plus quantifier
* `{5}` - exact quantifier
* `?` - optional quantifier
* `(expr)` - capturing subexpressions
* `(?:expr)` - non-capturing subexpressions
* `(?>expr)` - atomic subexpressions
* `\12` - backreferences
* `|` - alternation of two subexpressions
* `\?`, `\\`, `\n` - escaped characters
* `\w`, `\d`, `\S` - special character classes
* `.` - wildcard character
* `^`, `$` - anchors
* `[abc]`, `[^abc]`, `[a-z0-9]` - sets
* various flags like `ignore_case`, `dotall`, `extended` and `multiline`
* structured binding decomposition of `cx::regex_result`

Some features planned for the future:
* `++`, `?+`, `*+` - possessive quantifiers
* `{3, 5}` - range quantifier

## Usage
Below is an example of extracting data from a URL-string.
Notice that the generated regular expression is a type and not an object, since the library maps every
input regex string to a C++ type at compile-time. Invalid regular expression syntax will result
in a compilation error.

The input can be any object that satisfies the `cx::string_like` concept.
```cpp
using url_regex = cx::regex<R"((\w+):\/\/(?:(?:(\w+)?(?::(\w+))?@)?([\w.]+)(?::(\d+))?)?(?:(\/[-/\w]+)?\?([\w=&]+))?)">;

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

Searching and matching can also be done in `constexpr` if the input is known at
compile-time. Using `if constexpr` we can conditionally compile code based
on regex matching results. For example, we can generate functions with a different
return type based on some format pattern (`double` or `std::string`).
```cpp
template<cx::static_string const input>
constexpr auto parse()
{
    using test_number = cx::regex<R"([1-9]\d*(\.\d*)?(e(\+|-)?\d+(\.\d*)?)?)">;
    
    constexpr std::string_view url = static_cast<std::string_view>(input);
    if constexpr (test_number::match(url))
    {
        return std::stod(std::string(sv));
    }
    else
    {
        return std::string(sv);
    }
}
```

To avoid long and complex regular expressions, it is possible to combine two separate
`cx::regex` types into a union. The union behaves like an efficient alternation 
of all inner regular expressions.
```cpp
using phone_regex = cx::regex<R"(\d{3}-\d{3}-\d{3})">;
using email_regex = cx::regex<R"(([^@\s]+)@([^@\s]+))">;
using contact_regex = cx::make_union<email_regex, phone_regex>;
```
It is also possible to directly create a union, without intermediate `cx::regex`
types.
```cpp
using contact_regex = cx::regex_union<R"(\d{3}-\d{3}-\d{3})", R"(([^@\s]+)@([^@\s]+))">;
```

The library supports searching for multiple matches in a string. In this case,
the `cx::regex::find_all` method returns a lazy generator that will evaluate
on-demand all matches in the string. We can iterate through the generator
just like any standard container.
```cpp
using word_regex = cx::regex<R"([-a-z']+)", cx::flag::i>;

constexpr std::string_view words = "Let's iterate over these words!";
for (auto &&res : word_regex::find_all(words))
{
    std::cout << res << '\n';
}
```

C++17 structured binding declaration is also supported.
```cpp
using date_regex = cx::regex<R"((\d+)\/(\d+)\/(\d+))">;
constexpr std::string_view date = "07/08/2021";
auto [day, month, year] = date_regex::match(date);
std::cout << "Day: " << day << "\nMonth: " << month << "\nYear: " << year;
```
