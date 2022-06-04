#ifndef MREGEX_UNIVERSAL_CAPTURE_HPP
#define MREGEX_UNIVERSAL_CAPTURE_HPP

#include <type_traits>
#include <functional>

namespace meta
{
    /**
     * Wrapper class to handle perfect forwarding of lambda expression captures.
     */
    template <typename T>
    struct value_wrapper
    {
        using value_type = std::remove_reference_t<T>;

        template<typename U>
        constexpr explicit value_wrapper(U &&u)
        noexcept(std::is_nothrow_move_constructible_v<value_type>)
                : value{std::forward<U>(u)}
        {}

        [[nodiscard]] constexpr auto &get() & noexcept
        {
            return value;
        }

        [[nodiscard]] constexpr auto const &get() const & noexcept
        {
            return value;
        }

        [[nodiscard]] constexpr auto get() &&
        {
            return std::move(value);
        }

    private:
        value_type value;
    };

    // General case - use value semantics with perfect forwarding
    template<typename T>
    struct universal_capture : value_wrapper<T>
    {
        using value_wrapper<T>::value_wrapper;
    };

    // Specialized case - use reference semantics to avoid copying/moving
    template<typename T>
    struct universal_capture<T &> : std::reference_wrapper<T>
    {
        using std::reference_wrapper<T>::reference_wrapper;
    };

    template<typename T>
    universal_capture(T &&) -> universal_capture<T>;
}
#endif //MREGEX_UNIVERSAL_CAPTURE_HPP