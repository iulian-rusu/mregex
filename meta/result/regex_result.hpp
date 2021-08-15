#ifndef META_REGEX_RESULT_HPP
#define META_REGEX_RESULT_HPP

#include "owning_regex_result.hpp"
#include "../utility/tuple_map.hpp"

namespace meta
{
    template<std::size_t N>
    using result_base = regex_result_base<N, capture_storage<N>>;

    /**
     * Result that holds views of captured content.
     * If the orginal input string expires before this result object,
     * the behavior is undefined. Use the owning() method to create
     * an owning regex result.
     *
     * @tparam N    The number of capture groups (without the implicit 0 group)
     */
    template<std::size_t N>
    class regex_result : public result_base<N>
    {
        std::string_view input;
    public:
        using result_base<N>::captures;
        using result_base<N>::matched;

        template<typename Storage>
        constexpr regex_result(bool m, Storage &&storage, std::string_view str)
            : result_base<N>{m, std::forward<Storage>(storage)}, input{str}
        {}

        [[nodiscard]] constexpr std::size_t end() const noexcept
        {
            return std::get<0>(captures).end();
        }

        [[nodiscard]] auto owning() const
        {
            auto owning_captures = tuple_map(captures, [&](auto const &capture){
                return owning_regex_capture(capture, input);
            });
            return owning_regex_result{matched, std::move(owning_captures)};
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr decltype(auto) group() const noexcept
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(captures).evaluate(input);
        }

        /**
         * Method used for structured binding decomposition.
         * Prefer the group() method for manually extracting groups.
         */
        template<std::size_t ID>
        constexpr decltype(auto) get() const noexcept
        {
            static_assert(ID < N, "tuple element index out of bounds");
            return std::get<ID + 1>(captures).evaluate(input);
        }
    };

    template<typename Storage>
    regex_result(bool, Storage &&, std::string_view) -> regex_result<std::tuple_size_v<Storage> - 1>;
}

template<std::size_t N>
std::ostream &operator<<(std::ostream &os, meta::regex_result<N> const & result)
{
    return os << result.template group<0>();
}

namespace std
{
    template <std::size_t N>
    struct tuple_size<meta::regex_result<N>> : std::integral_constant<std::size_t, N> {};

    template <std::size_t ID, std::size_t N>
    struct tuple_element<ID, meta::regex_result<N>>
    {
        using type = std::string_view;
    };
}
#endif //META_REGEX_RESULT_HPP