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

    auto match = my_regex::match(list);
    std::cout << "Log level: " << match.group<"level">() << '\n';
    std::cout << "Message: '" << match.group<"msg">() << "'\n";
}