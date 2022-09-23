#include <iostream>
#include <mregex.hpp>

int main()
{
    using url_regex = meta::regex<R"((\w+):\/\/(?:(?:(\w+)(?::(\w+))?@)?([\w.]+)(?::(\d+))?)?(?:(\/[-/\w]+)?\?([\w=&]+))?)">;

    // Matching can also be done at compile-time
    constexpr std::string_view url = "https://username:password@hostname.com:8080/path/to/resource?id=12345";
    constexpr auto result = url_regex::match(url);
    // The matched() method can be used to check if the regex has matched
    std::cout << "Matched: " << result.matched() << '\n';
    // Captured content can be accessed with the group<>() method
    // Group 0 is the entire regex, other captures are indexed from 1
    std::cout << "Schema:  " << result.group<1>() << '\n';
    std::cout << "User:    " << result.group<2>() << '\n';
    std::cout << "Pass:    " << result.group<3>() << '\n';
    std::cout << "Host:    " << result.group<4>() << '\n';
    std::cout << "Port:    " << result.group<5>() << '\n';
    std::cout << "Path:    " << result.group<6>() << '\n';
    std::cout << "Query:   " << result.group<7>() << "\n";
}