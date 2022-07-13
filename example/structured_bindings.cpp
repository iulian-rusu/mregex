#include <iostream>
#include <mregex.hpp>

int main()
{
    // This regex has 3 capture groups
    // Its match result can be decomposed into 3 local variables
    using date_regex = meta::regex<R"((\d{1,2})/(\d{1,2})/(\d{2,4}))">;

    auto const get_date_info = []() {
        std::string date = "Today is 07/08/2021";
        // By default, all methods return a view into the original input string
        // In this case, the original std::string expires when exiting the function
        // By calling .own(), a deep copy of all captures is performed (with heap allocations)
        return date_regex::search(date).own();
    };
    // Each variable corresponds to one capturing group, in order of their definition in the regex
    auto [day, month, year] = get_date_info();
    std::cout << "Day: " << day << "\nMonth: " << month << "\nYear: " << year << '\n';
}