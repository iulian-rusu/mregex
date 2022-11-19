#include <iostream>
#include <mregex.hpp>

int main()
{
    // This regex has 3 capture groups
    // Its match result can be decomposed into 3 local variables
    using date_regex = meta::regex<R"((\d{1,2})/(\d{1,2})/(\d{2,4}))">;

    auto const get_date_info = []() -> meta::regex_result_t<date_regex> {
        std::string date = "Today is 07/08/2021";
        // By default, all methods return results that have views into the original input string
        // In this case, the lifetime of the original std::string is too short
        // By calling as_memory_owner(), a deep copy of all captures is performed (with heap allocations)
        return date_regex::search(date).as_memory_owner();
    };
    // Each variable corresponds to one capturing group, in order of their definition in the regex
    auto [day, month, year] = get_date_info();
    std::cout << "Day: " << day << "\nMonth: " << month << "\nYear: " << year << '\n';
}