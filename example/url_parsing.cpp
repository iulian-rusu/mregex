#include <iostream>
#include <mregex.hpp>

int main()
{
    using url_regex = meta::regex<R"((\w+):\/\/(?:(?:(\w+)(?::(\w+))?@)?([\w.]+)(?::(\d+))?)?(?:(\/[-/\w]+)?\?([\w=&]+))?)">;

    // Matching can also be done at compile-time
    constexpr std::string_view url = "https://username:password@hostname.com:8080/path/to/resource?id=12345";
    constexpr auto match = url_regex::match(url);
    // The status() method can be used to check if the regex has matched
    std::cout << "Is valid URL: " << match.status() << '\n';
    // Captured content can be accessed with the group<>() method
    // Group 0 is the entire regex, other captures are indexed from 1
    std::cout << "Scheme:\t" << match.group<1>() << '\n';
    std::cout << "User:\t" << match.group<2>() << '\n';
    std::cout << "Pass:\t" << match.group<3>() << '\n';
    std::cout << "Host:\t" << match.group<4>() << '\n';
    std::cout << "Port:\t" << match.group<5>() << '\n';
    std::cout << "Path:\t" << match.group<6>() << '\n';
    std::cout << "Query:\t" << match.group<7>() << "\n";
}