#include "ctr.h"

int main()
{
    using s = ctr::stack<char, int, int>;
    static_assert(std::is_same_v<ctr::stack<double, char, int, int>, typename s::push<double>>);
    static_assert(std::is_same_v<ctr::stack<double, long, char, int, int>, typename s::push<ctr::stack<double, long>>>);

    static_assert(ctr::parser<"(((abcd)?x)*)|(asd+)">::is_valid);
}
