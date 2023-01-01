#include <functional>
#include <iostream>
#include <mregex.hpp>

namespace detail
{
    // Until we get std::optional::transform in C++23, we have to implement this manually
    template<typename Opt, typename F>
    constexpr auto transform(Opt &&opt, F &&f)
    {
        using result_type = std::remove_reference_t<decltype(std::invoke(std::forward<F>(f), *std::forward<Opt>(opt)))>;

        if (opt.has_value())
            return std::optional{std::invoke(std::forward<F>(f), *std::forward<Opt>(opt))};
        return std::optional<result_type>{std::nullopt};
    }
}

using my_regex = meta::regex<R"(Today is (?<day>[A-Z][a-z]+))">;

constexpr auto try_parse_day(std::string_view text) -> std::optional<std::string_view>
{
    // Regex results can be converted to std::optional
    // This makes it easier to use standard interface methods like `transform`, `and_then`, `or_else` etc.
    return detail::transform(
        my_regex::match(text).as_optional(),
        meta::get_group_named<"day">
    );
}

int main()
{
    if (auto opt_match = try_parse_day("Today is Monday"))
        std::cout << "Matched day: " << *opt_match << '\n';
    else
        std::cout << "Not matched :(\n";
}