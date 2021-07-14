#include "cxregex.h"

struct phone
{
    std::string number;

    constexpr explicit phone(auto &&s) : number(std::forward<decltype(s)>(s))
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

    constexpr std::string_view sv = static_cast<std::string_view>(input);
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
    // Basic URL parsing
    using url_regex = cx::regex<R"((\w+):\/\/(?:(?:(\w+)?(?::(\w+))?@)?([\w.]+)(?::(\d+))?)?(?:(\/[-/\w]+)?\?([\w=&]+))?)">;
    constexpr std::string_view url = "https://username:password@hostname.com:8080/path/to/resource?id=12345";
    constexpr auto match_res = url_regex::match(url);
    std::cout << "Scheme:\t" << match_res.get<1>() << '\n';
    std::cout << "User:\t" << match_res.get<2>() << '\n';
    std::cout << "Pass:\t" << match_res.get<3>() << '\n';
    std::cout << "Host:\t" << match_res.get<4>() << '\n';
    std::cout << "Port:\t" << match_res.get<5>() << '\n';
    std::cout << "Path:\t" << match_res.get<6>() << '\n';
    std::cout << "Query:\t" << match_res.get<7>() << '\n';

    // Changing function return type depending on input string format
    double number = parse<"123.3e-10">();
    std::cout << number << '\n';
    phone my_phone = parse<"(+373) 75-44-98">();
    std::cout << my_phone << '\n';
    std::string str_value = parse<"hello">();
    std::cout << str_value << '\n';

    // Lazy iteration over a collection of results
    using word_regex = cx::regex<R"([-a-z']+)", cx::flag::ignore_case>;
    constexpr std::string_view words = "Let's iterate over these words!";
    for (auto &&res : word_regex::find_all(words))
    {
        std::cout << res.get<0>() << '\n';
    }
}