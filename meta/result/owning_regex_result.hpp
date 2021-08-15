#ifndef META_OWNING_REGEX_RESULT_HPP
#define META_OWNING_REGEX_RESULT_HPP

#include <iosfwd>
#include <tuple>
#include "regex_result_base.hpp"

namespace meta
{
    template<std::size_t N>
    using owning_result_base = regex_result_base<N, owning_capture_storage<N>>;

    /**
     * Regex result that holds ownership of captured content.
     *
     * @tparam N    The number of capture groups (without the implicit 0 group)
     */
    template<std::size_t N>
    class owning_regex_result : public owning_result_base<N>
    {
    public:
        using owning_result_base<N>::captures;
        using owning_result_base<N>::matched;

        template<typename Storage>
        owning_regex_result(bool m, Storage &&storage)
            : owning_result_base<N>{m, std::forward<Storage>(storage)}
        {}

        template<std::size_t ID>
        [[nodiscard]] decltype(auto) group() const noexcept
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(captures).get();
        }

        /**
         * Method used for structured binding decomposition.
         * Prefer the group() method for manually extracting groups.
         */
        template<std::size_t ID>
        decltype(auto) get() const noexcept
        {
            static_assert(ID < N, "tuple element index out of bounds");
            return std::get<ID + 1>(captures).get();
        }
    };

    template<typename Storage>
    owning_regex_result(bool, Storage &&) -> owning_regex_result<std::tuple_size_v<Storage> - 1>;
}

template<std::size_t N>
std::ostream &operator<<(std::ostream &os, meta::owning_regex_result<N> const & result)
{
    return os << result.template group<0>();
}

namespace std
{
    template <std::size_t N>
    struct tuple_size<meta::owning_regex_result<N>> : std::integral_constant<std::size_t, N> {};

    template <std::size_t ID, std::size_t N>
    struct tuple_element<ID, meta::owning_regex_result<N>>
    {
        using type = std::string_view;
    };
}
#endif //META_OWNING_REGEX_RESULT_HPP
