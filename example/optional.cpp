#include <functional>
#include <iostream>
#include <mregex.hpp>

namespace
{
    template<typename Opt, typename Func>
    constexpr auto transform(Opt &&opt, Func &&func)
    {
        using result_type = std::remove_reference_t<decltype(std::invoke(std::forward<Func>(func), *std::forward<Opt>(opt)))>;

        if (!opt.has_value())
            return std::optional<result_type>{std::nullopt};
        return std::optional{std::invoke(std::forward<Func>(func), *std::forward<Opt>(opt))};
    }

    template<meta::static_string Name>
    inline constexpr auto get_group = []<typename Result>(Result &&res) noexcept -> decltype(auto) {
        return std::forward<Result>(res).template group<Name>();
    };

    using my_regex = meta::regex<R"(Today is (?<day>[A-Z][a-z]+))">;

    constexpr auto try_parse_day(std::string_view text) -> std::optional<std::string_view>
    {
        // Regex results can be converted to std::optional
        // This makes it easier to use standard interface methods like transform, and_then, or_else etc.
        return transform(
            my_regex::match(text).as_optional(),
            get_group<"day">
        );
    }
}


int main()
{
    auto inputs = std::array{"Today is Monday", "something else"};
    for (auto input : inputs)
    {
        if (auto opt_match = try_parse_day(input))
            std::cout << "Matched day: " << *opt_match << '\n';
        else
            std::cout << "Did not match: " << input << '\n';
    }
}