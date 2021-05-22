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
    using test_float = cx::regex<R"([1-9]+(\.\d*)?(e(\+|-)?\d+(\.\d*)?)?)">;
    using test_phone = cx::regex<R"((\(\+\d+\))? ?\d+-\d+(-\d+))">;
    constexpr std::string_view sv(input.buffer, input.length());

    if constexpr (test_float::match(sv))
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
    double number = parse<"123.3e-10">();
    std::cout << number << '\n';

    phone my_phone = parse<"(+373) 75-44-98">();
    std::cout << my_phone << '\n';

    std::string str_value = parse<"hello">();
    std::cout << str_value << '\n';

    // extracting data with capture groups
    using date_regex = cx::regex<R"(([0-9]+) ?- ?([A-z0-9]+) ?- ?([0-9]+))">;
    std::string_view date_sv = "Today is 05-april-2002!";
    auto res = date_regex::search(date_sv);
    std::cout << "Date:\t" << res.get<0>() << '\n';
    std::cout << "Day:\t" << res.get<1>() << '\n';
    std::cout << "Month:\t" << res.get<2>() << '\n';
    std::cout << "Year:\t" << res.get<3>() << '\n';
}