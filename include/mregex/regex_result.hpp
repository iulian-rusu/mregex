#ifndef MREGEX_REGEX_RESULT_HPP
#define MREGEX_REGEX_RESULT_HPP

#include <iosfwd>
#include <tuple>
#include <mregex/utility/tuple_helpers.hpp>
#include <mregex/regex_capture.hpp>

namespace meta
{
    /**
     * Class returned by all regex matching/searching functions.
     *
     * @tparam NameSpec The type that contains the capture name specification
     * @tparam Storage  The storage type used to hold the captures
     */
    template<typename NameSpec, capture_storage Storage>
    struct basic_regex_result;

    /**
     * Result that holds views into the captured content.
     * If the orginal input string expires before this result object,
     * the behavior is undefined.
     */
    template<typename NameSpec, std::forward_iterator Iter>
    using regex_result_view = basic_regex_result<NameSpec, regex_capture_view_storage<NameSpec, Iter>>;

    /**
     * Result that holds ownership of captured content.
     */
    template<typename NameSpec>
    using regex_result = basic_regex_result<NameSpec, regex_capture_storage<NameSpec>>;

    template<typename NameSpec, capture_storage Storage>
    struct basic_regex_result
    {
        using storage_type = Storage;
        using capture_type = std::tuple_element_t<0, storage_type>;

        static constexpr std::size_t capture_count = std::tuple_size_v<storage_type> - 1;

        template<typename S>
        constexpr basic_regex_result(bool matched, S &&captures)
        noexcept(std::is_nothrow_move_constructible_v<storage_type>)
            : matched{matched}, captures{std::forward<S>(captures)}
        {}

        [[nodiscard]] constexpr bool status() const noexcept
        {
            return matched;
        }

        [[nodiscard]] constexpr std::size_t length() const noexcept
        {
            return std::get<0>(captures).length();
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return  std::get<0>(captures).begin();
        }

        [[nodiscard]] constexpr auto end() const noexcept
        {
            return  std::get<0>(captures).end();
        }

        /**
         * Performs a deep copy of all regex captures and returns
         * a regex result container that owns them.
         *
         * @return  A new regex_result object that holds ownership of captures
         */
        [[nodiscard]] auto own() const
        requires is_capture_view_v<capture_type>
        {
            auto owning_captures = generate_tuple(captures, [](auto const &capture) {
                return regex_capture{capture};
            });
            return regex_result<NameSpec>{matched, std::move(owning_captures)};
        }

        /**
         * Returns the capturing group with the specified number.
         * The method is specialized to move the captures if this object
         * is an expiring value.
         *
         * @tparam ID   The number of the requested capturing group
         * @return      The capturing group
         */
        template<std::size_t ID>
        [[nodiscard]] constexpr auto &group() & noexcept
        {
            assert_valid_group<ID>();
            return std::get<ID>(captures);
        }

        /**
         * Returns the capturing group with the specified name.
         * The method is specialized to move the captures if this object
         * is an expiring value.
         *
         * @tparam Name The name of the requested capturing group
         * @return      The capturing group
         */
        template<static_string Name>
        [[nodiscard]] constexpr auto &group() & noexcept
        {
            return std::get<named_capture_type_for<storage_type, symbol::name<Name>>>(captures);
        }

        /**
         * More overloads are required because C++ doesn't allow deducing
         * the value category of `this` inside a method.
         * @see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0847r4.html
         */

        template<std::size_t ID>
        [[nodiscard]] constexpr auto const &group() const & noexcept
        {
            assert_valid_group<ID>();
            return std::get<ID>(captures);
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr auto &&group() && noexcept
        {
            assert_valid_group<ID>();
            return std::get<ID>(captures);
        }

        template<static_string Name>
        [[nodiscard]] constexpr auto const &group() const & noexcept
        {
            return std::get<named_capture_type_for<storage_type, symbol::name<Name>>>(captures);
        }

        template<static_string Name>
        [[nodiscard]] constexpr auto &&group() && noexcept
        {
            return std::get<named_capture_type_for<storage_type, symbol::name<Name>>>(captures);
        }

        /**
         * Tuple-like interface for structured binding decomposition.
         * Use the group() method for extracting captures.
         */
        template<std::size_t ID>
        constexpr decltype(auto) get() noexcept
        {
            assert_valid_group<ID + 1>();
            return std::get<ID + 1>(captures);
        }

        template<std::size_t ID>
        constexpr decltype(auto) get() const noexcept
        {
            assert_valid_group<ID + 1>();
            return std::get<ID + 1>(captures);
        }

        constexpr bool operator==(bool value) const noexcept
        {
            return matched == value;
        }

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr explicit(false) operator std::string_view() const noexcept
        requires std::is_convertible_v<capture_type, std::string_view>
        {
            return static_cast<std::string_view>(group<0>());
        }

    private:
        bool matched;
        storage_type captures;

        template<std::size_t ID>
        static constexpr void assert_valid_group() noexcept
        {
            static_assert(ID <= capture_count, "capturing group does not exist");
        }
    };
}

template<typename NameSpec, typename Storage>
std::ostream &operator<<(std::ostream &os, meta::basic_regex_result<NameSpec, Storage> const &result)
{
    return os << result.template group<0>();
}

namespace std
{
    template<typename NameSpec, typename Storage>
    struct tuple_size<meta::basic_regex_result<NameSpec, Storage>>
    {
        static constexpr size_t value = meta::basic_regex_result<NameSpec, Storage>::capture_count;
    };

    template<size_t ID, typename NameSpec, typename Storage>
    struct tuple_element<ID, meta::basic_regex_result<NameSpec, Storage>>
    {
        using type = tuple_element_t<ID + 1, Storage>;
    };
}
#endif //MREGEX_REGEX_RESULT_HPP