#include <iostream>
#include <mregex.hpp>

int main()
{
    // This regular expression has 3 capturing groups: 1 and 2 are unnamed, while 3 is named "country_name"
    using my_regex = meta::regex<R"(Name: (\w+), age: (\d+), country: (?<country_name>\w+);)">;

    std::string_view str = "Name: John, age: 42, country: Australia;";
    auto match = my_regex::match(str);
    if (match)
    {
        // The group<>() method returns a special object that contains the captured content
        auto name = match.group<1>();
        // Regex captures are also implicitly convertible to std::string_view (when using contiguous iterators)
        std::string_view age = match.group<2>();
        // Named capturing groups can be accessed either by index or by their name
        auto country = match.group<"country_name">();
        std::cout << name << ", " << age << ", lives in " << country << '\n';
    }
}