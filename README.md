# Constexpr Regex in C++

## Summary
This is a minimalist `constexpr` implementation of a regular expression
library using template metaprogramming in C++20. The implementation relies
heavily on the declarative behavior of templates in C++ and uses 
functional techniques to implement the parser and generate the abstract syntax 
tree from an input string.

## Usage
The provided interface is extremely simple and follows most regular expression
libraries. Below is an example of extracting data from a URL-string.
Notice that the generated regular expression is a type and not an object, since the library maps every
input regex string to a C++ type at compile-time. Invalid regular expression syntax will result
in a compilation error.

The input can be any object that satisfies the `cx::string_like` concept.
```cpp
using url_regex = cx::regex<R"((\w+):\/\/(((\w+):(\w+)?@)?([\w.]+)(:(\d+))?)?(\/([-/\w]+)?\?([\w=&]+))?)">;

std::string_view sv = "https://root:pass@youtube.com:80/path/watch?video=123123";
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
compile-time.