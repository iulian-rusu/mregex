#include <forward_list>
#include <iostream>
#include <mregex.hpp>

int main()
{
    using my_regex = meta::regex<R"(\[(?<level>[A-Z]++)\]: (?<msg>.++))">;

    // Matching can be done on any forward range that contains char-like elements
    // In this example, matching is done on a linked list
    std::string_view message = "[ERROR]: Segmentation fault (core dumped)";
    std::forward_list<char> list{message.begin(), message.end()};

    // The result type of matching can be named explicitly using this type alias
    using result_type = meta::regex_result_view_t<my_regex, std::forward_list<char>::const_iterator>;

    result_type result = my_regex::match(list);
    std::cout << "Log level: " << result.group<"level">() << '\n';
    std::cout << "Message: '" << result.group<"msg">() << "'\n";
}