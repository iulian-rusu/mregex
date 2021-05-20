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
    using test_float = cx::regex<R"(\d+(\.\d*)?(e(\+|-)?\d+(\.\d*)?)?)">;
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
        return std::string(std::string(sv));
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
}