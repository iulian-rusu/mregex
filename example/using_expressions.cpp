#include <iostream>
#include <mregex.hpp>

// Alias for convenience
namespace xpr = meta::xpr;

int main()
{
    /**
     * Instead of defining the entire regex as a string, we can build it from smaller components, like a C++ expression.
     *
     * This example will use overloaded operators to replace standard regex operators.
     * You can also use functions like xpr::concat() or xpr::either() to achieve the same effect.
     *
     * Concatenation is done with the >> operator.
     * Alternation is done with the | operator.
     * The Kleene star and plus quantifiers are achieved with unary * and + operators.
     * Negation of character classes and lookarounds is done with the ! operator.
     * Flags can be added after defining the regex using the / operator.
     *
     * Once an expression has been modified by flags, it cannot be composed with other expressions!
     */

    // Opting in for operators is done by importing the namespace xpr::operators
    using namespace xpr::operators;

    // Constructing a regex equivalent to '(?:ftp|ssh|https?)://(?<domain>:[-.a-z])+.com(?:/\S*)?'
    auto schema = xpr::str<"ftp"> | xpr::str<"ssh"> | xpr::regex<"https?">;
    auto domain = +xpr::regex<"[-.a-z]"> >> xpr::str<".com">;
    auto path = xpr::chr<'/'> >> *!xpr::whitespace;
    auto url = schema >> xpr::str<"://"> >> xpr::capture<1, "domain">(domain) >> xpr::maybe(path);

    // The resulting object has the same interface as a meta::regex type
    if (auto match = url.match("https://google.com"))
        std::cout << "Matched domain: " << match.group<"domain">() << '\n';
    else
        std::cout << "Not matched :(\n";
}