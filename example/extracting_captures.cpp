#include <iostream>
#include <mregex.hpp>

int main()
{
    using my_regex = meta::regex<R"(Name: (\w+), age: (\d+);)">;

    std::string_view str = "Name: Jeff, age: 30;";
    auto match = my_regex::match(str);
    if (match)
    {
        // The group<>() method returns a special regex capture object
        auto name = match.group<1>();
        // It is also implicitly convertible to a std::string or std::string_view
        std::string_view age = match.group<2>();
        std::cout << "The name is " << name << " and the age is " << age << '\n';
    }
}