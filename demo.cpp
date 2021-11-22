#include <iostream>
#include <mregex.hpp>

int main()
{
    // Basic URL parsing
    using url_regex = meta::regex<R"((\w+):\/\/(?:(?:(\w+)?(?::(\w+))?@)?([\w.]+)(?::(\d+))?)?(?:(\/[-/\w]+)?\?([\w=&]+))?)">;
    constexpr std::string_view url = "https://username:password@hostname.com:8080/path/to/resource?id=12345";
    constexpr auto match_res = url_regex::match(url);
    std::cout << "Scheme:\t" << match_res.group<1>() << '\n';
    std::cout << "User:\t" << match_res.group<2>() << '\n';
    std::cout << "Pass:\t" << match_res.group<3>() << '\n';
    std::cout << "Host:\t" << match_res.group<4>() << '\n';
    std::cout << "Port:\t" << match_res.group<5>() << '\n';
    std::cout << "Path:\t" << match_res.group<6>() << '\n';
    std::cout << "Query:\t" << match_res.group<7>() << "\n\n";

    // Lazy iteration over all matches
    using word_regex = meta::regex<R"(\w+(?=\W))", meta::flag::ungreedy>;
    std::string words = "Find all word-like sequences in this string!";
    for (auto &&res : word_regex::range(words))
        std::cout << res << '\n';
    std::cout << '\n';

    // Using structured bindings
    auto const get_date_info = []() {
        using date_regex = meta::regex<R"((\d{1,2})/(\d{1,2})/(\d{2,4}))">;
        std::string date = "Today is 07/08/2021";
        return date_regex::search(date).own();
    };
    // Results are decomposed into rvalue references to std::string and moved into new variables
    auto [day, month, year] = get_date_info();
    std::cout << "Day: " << day << "\nMonth: " << month << "\nYear: " << year;
}