#include <iostream>
#include <mregex.hpp>

int main()
{
    // Basic URL parsing
    using url_regex = meta::regex<R"((\w+):\/\/(?:(?:(\w+)?(?::(\w+))?@)?([\w.]+)(?::(\d+))?)?(?:(\/[-/\w]+)?\?([\w=&]+))?)">;
    constexpr std::string_view url = "https://username:password@hostname.com:8080/path/to/resource?id=12345";
    constexpr auto match_res = url_regex::match(url);
    // Extract captured content with the group<>() method
    // Group 0 is the entire regex, captures are numbered from 1
    std::cout << "Scheme:\t" << match_res.group<1>() << '\n';
    std::cout << "User:\t" << match_res.group<2>() << '\n';
    std::cout << "Pass:\t" << match_res.group<3>() << '\n';
    std::cout << "Host:\t" << match_res.group<4>() << '\n';
    std::cout << "Port:\t" << match_res.group<5>() << '\n';
    std::cout << "Path:\t" << match_res.group<6>() << '\n';
    std::cout << "Query:\t" << match_res.group<7>() << "\n";
}