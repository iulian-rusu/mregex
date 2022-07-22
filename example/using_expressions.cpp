#include <iostream>
#include <mregex.hpp>

int main()
{
    /**
     * Instead of defining the entire regex as a string, we can build it from smaller components, like a C++ expression.
     *
     * This example will use overloaded operators to replace standard regex operators.
     * You can also use functions like expr::concat() or expr::either() to achieve the same effect.
     *
     * Concatenation is done with the >> operator.
     * Alternation is done with the | operator.
     * The Kleene star and plus quantifiers are achieved with unary * and + operators.
     * Negation of character classes and lookarounds is done with the ! operator.
     * Flags can be added after defining the regex using the / operator.
     */

    // Opting in for operators is done by importing the namespace expr::operators
    using namespace expr::operators;

    // Equivalent to (?:ftp|ssh|https?)://(?:[-.a-z])+.com(?:/\S*)?
    auto pattern = (
        (expr::str<"ftp"> | expr::str<"ssh"> | expr::regex<"https?">) >>
        expr::str<"://"> >> +expr::regex<"[-.a-z]"> >> expr::str<".com"> >>
        expr::maybe(
            expr::chr<'/'>,
            *!expr::whitespace
        )
    ) / expr::flag::icase;

    // The pattern object has the same interface as a meta::regex type
    if (auto match = pattern.match("hTtPs://GoOgLe.CoM"))
        std::cout << "Matched!\n";
    else
        std::cout << "Not matched :(\n";
}