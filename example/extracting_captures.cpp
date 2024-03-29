#include <iostream>
#include <mregex.hpp>

int main()
{
    // This regular expression has 3 capturing groups: 1 and 2 are unnamed, while 3 is named "country_name"
    using my_regex = meta::regex<R"(Name: (\w+), age: (\d+), country: (?<country_name>\w+);)">;

    std::string_view str = "Name: John, age: 42, country: Australia;";
    if (auto result = my_regex::match(str))
    {
        // The group<>() method returns a special object that contains the captured content
        auto name = result.group<1>();
        // Regex captures are also implicitly convertible to std::string_view (when using contiguous iterators)
        std::string_view age = result.group<2>();
        // Named capturing groups can be accessed either by index or by their name
        auto country = result.group<"country_name">();
        std::cout << name << ", " << age << ", lives in " << country << '\n';
    }
}