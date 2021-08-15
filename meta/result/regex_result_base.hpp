#ifndef META_REGEX_RESULT_BASE_HPP
#define META_REGEX_RESULT_BASE_HPP

#include "regex_capture.hpp"

namespace meta
{
    /**
     * Base class for result returned by all regex matching/searching functions.
     *
     * @tparam N    The number of capture groups (without the implicit 0 group)
     */
    template<std::size_t N, typename Storage>
    class regex_result_base
    {
    protected:
        bool matched{};
        Storage captures;
    public:
        template<typename Strg>
        constexpr regex_result_base(bool m, Strg &&storage)
        requires std::is_convertible_v<Storage, Strg>
            : matched{m}, captures{std::forward<Strg>(storage)}
        {}

        constexpr operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }
    };

    template<typename Storage>
    regex_result_base(bool, Storage &&, std::string_view) -> regex_result_base<std::tuple_size_v<Storage> - 1, Storage>;
}
#endif //META_REGEX_RESULT_BASE_HPP
