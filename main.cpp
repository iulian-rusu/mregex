#include <iostream>
#include "cxregex.h"

struct phone
{
    std::string number;

    constexpr explicit phone(auto &&sv) : number(std::forward<decltype(sv)>(sv))
    {}
};

std::ostream &operator<<(std::ostream &out, phone const &p)
{
    return out << p.number;
}

template<cx::static_string const input>
constexpr auto parse()
{
    using test_number = cx::regex<R"([1-9]+(\.\d*)?(e(\+|-)?\d+(\.\d*)?)?)">;
    using test_phone = cx::regex<R"((\(\+\d+\))? ?\d+-\d+(-\d+))">;

    constexpr std::string_view sv(input.buffer, input.length());
    if constexpr (test_number::match(sv))
    {
        return std::stod(std::string(sv));
    }
    else if constexpr (test_phone::match(sv))
    {
        return phone(sv);
    }
    else
    {
        return std::string(sv);
    }
}

int main()
{
    // change function return type depending on regex parsing outcome
    double number = parse<"123.3e-10">();
    std::cout << number << '\n';
    phone my_phone = parse<"(+373) 75-44-98">();
    std::cout << my_phone << '\n';
    std::string str_value = parse<"hello">();
    std::cout << str_value << '\n';

    // simple URL parsing example
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
}