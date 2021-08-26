#ifndef META_UNIVERSAL_CAPTURE_HPP
#define META_UNIVERSAL_CAPTURE_HPP

#include <type_traits>
#include <functional>

namespace meta
{
    /**
     * Wrapper class to handle perfect forwarding of lambda expression captures.
     * Adapted from https://vittorioromeo.info/index/blog/capturing_perfectly_forwarded_objects_in_lambdas.html
     */
    template <typename T>
    class value_wrapper
    {
        using value_type = std::remove_reference_t<T>;

        value_type value;

    public:
        template <typename U>
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
    constexpr auto make_universal_capture(T &&value)
    {
        return universal_capture<T>{std::forward<T>(value)};
    }
}
#endif //META_UNIVERSAL_CAPTURE_HPP