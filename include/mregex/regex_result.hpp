#ifndef MREGEX_REGEX_RESULT_HPP
#define MREGEX_REGEX_RESULT_HPP

#include <iosfwd>
#include <optional>
#include <tuple>
#include <mregex/utility/tuple.hpp>
#include <mregex/regex_capture.hpp>

namespace meta
{
    /**
     * Class returned by all regex matching/searching functions.
     *
     * @tparam CaptureStorage   The storage type used to hold the captures
     * @tparam NameSpec         A type that contains the capture name specification
     */
    template<capture_storage CaptureStorage, typename NameSpec>
    struct basic_regex_result;

    /**
     * Result that holds views into the captured content.
     * If the orginal input string expires before this result object,
     * the behavior is undefined.
     */
    template<std::forward_iterator Iter, typename NameSpec>
    using regex_result_view = basic_regex_result<regex_capture_view_storage<Iter, NameSpec>, NameSpec>;

    /**
     * Result that holds ownership of captured content.
     */
    template<typename NameSpec>
    using regex_result = basic_regex_result<regex_capture_storage<NameSpec>, NameSpec>;

    template<capture_storage CaptureStorage, typename NameSpec>
    struct basic_regex_result
    {
        using capture_storage_type = CaptureStorage;
        using implicit_capture_type = std::tuple_element_t<0, capture_storage_type>;

        static constexpr bool is_view = is_capture_view<implicit_capture_type>;
        static constexpr std::size_t capture_count = std::tuple_size_v<capture_storage_type> - 1;

        constexpr basic_regex_result() noexcept = default;

        template<capture_storage Captures>
        constexpr basic_regex_result(Captures &&captures, bool matched)
        noexcept(std::is_nothrow_constructible_v<capture_storage_type, Captures &&>)
            : _captures{std::forward<Captures>(captures)}, _matched{matched}
        {}

        [[nodiscard]] constexpr bool matched() const noexcept
        {
            return _matched;
        }

        [[nodiscard]] constexpr std::size_t length() const noexcept
        {
            return std::get<0>(_captures).length();
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return std::get<0>(_captures).begin();
        }

        [[nodiscard]] constexpr auto end() const noexcept
        {
            return std::get<0>(_captures).end();
        }

        /**
         * Performs a deep copy of all regex captures and returns
         * a regex result container that owns the memory.
         *
         * @return  A new regex result object that holds ownership of captures
         */
        [[nodiscard]] auto as_memory_owner() const
        requires is_view
        {
            auto owned_captures = transform_tuple(_captures, [](auto const &capture) {
                return regex_capture{capture};
            });
            return regex_result<NameSpec>{std::move(owned_captures), _matched};
        }

        /**
         * Converts the regex result object to an instance of std::optional.
         * The returned optional will be empty if the regex did not match.
         *
         * @return A new instance of std::optional that contains the regex result object
         */
        [[nodiscard]] constexpr auto as_optional() & noexcept(is_view)
        {
            return forward_self_as_optional(*this);
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
            return get_group_by_index<ID>(_captures);
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
            return get_group_by_name<Name>(_captures);
        }

        /**
         * More overloads are required because C++ doesn't allow deducing
         * the value category of `this` inside a method.
         * @see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0847r4.html
         */

        [[nodiscard]] constexpr auto as_optional() const & noexcept(is_view)
        {
            return forward_self_as_optional(*this);
        }

        [[nodiscard]] constexpr auto as_optional() && noexcept
        {
            return forward_self_as_optional(std::move(*this));
        }

        [[nodiscard]] constexpr auto as_optional() const && noexcept(is_view)
        {
            return forward_self_as_optional(std::move(*this));
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr auto const &group() const & noexcept
        {
            return get_group_by_index<ID>(_captures);
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr auto &&group() && noexcept
        {
            return get_group_by_index<ID>(std::move(_captures));
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr auto const &&group() const && noexcept
        {
            return get_group_by_index<ID>(std::move(_captures));
        }

        template<static_string Name>
        [[nodiscard]] constexpr auto const &group() const & noexcept
        {
            return get_group_by_name<Name>(_captures);
        }

        template<static_string Name>
        [[nodiscard]] constexpr auto &&group() && noexcept
        {
            return get_group_by_name<Name>(std::move(_captures));
        }

        template<static_string Name>
        [[nodiscard]] constexpr auto const &&group() const && noexcept
        {
            return get_group_by_name<Name>(std::move(_captures));
        }

        /**
         * Tuple-like interface for structured binding decomposition.
         * Use the group() method for extracting captures.
         */
        template<std::size_t ID>
        [[nodiscard]] constexpr auto &get() & noexcept
        {
            return get_group_by_index<ID + 1>(_captures);
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr auto const &get() const & noexcept
        {
            return get_group_by_index<ID + 1>(_captures);
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr auto &&get() && noexcept
        {
            return get_group_by_index<ID + 1>(std::move(_captures));
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr auto const &&get() const && noexcept
        {
            return get_group_by_index<ID + 1>(std::move(_captures));
        }

        [[nodiscard]] constexpr bool operator==(bool value) const noexcept
        {
            return _matched == value;
        }

        constexpr explicit operator bool() const noexcept
        {
            return _matched;
        }

        constexpr explicit(false) operator std::string_view() const noexcept
        requires std::is_convertible_v<implicit_capture_type, std::string_view>
        {
            return static_cast<std::string_view>(group<0>());
        }

    private:
        template<typename Self>
        static constexpr auto forward_self_as_optional(Self &&self)
        noexcept(is_view || std::is_rvalue_reference_v<Self &&>)
        -> std::optional<std::remove_cvref_t<Self>>
        {
            if (self.matched())
                return std::optional{std::forward<Self>(self)};
            return std::nullopt;
        }

        template<std::size_t ID, capture_storage Captures>
        static constexpr decltype(auto) get_group_by_index(Captures &&captures) noexcept
        {
            static_assert(ID <= capture_count, "capturing group does not exist");
            return std::get<ID>(std::forward<Captures>(captures));
        }

        template<static_string Name, capture_storage Captures>
        static constexpr decltype(auto) get_group_by_name(Captures &&captures) noexcept
        {
            using capture_type = rename_capture_t<implicit_capture_type, symbol::name<Name>>;
            return std::get<capture_type>(std::forward<Captures>(captures));
        }

        capture_storage_type _captures;
        bool _matched;
    };

    /**
     * Functors used to project a regex result to one of its capture groups.
     */

    template<std::size_t ID>
    inline constexpr auto get_group = []<typename Result>(Result &&res) noexcept -> decltype(auto) {
        return std::forward<Result>(res).template group<ID>();
    };

    template<static_string Name>
    inline constexpr auto get_group_named = []<typename Result>(Result &&res) noexcept -> decltype(auto) {
        return std::forward<Result>(res).template group<Name>();
    };
}

template<meta::capture_storage CaptureStorage, typename NameSpec>
std::ostream &operator<<(std::ostream &os, meta::basic_regex_result<CaptureStorage, NameSpec> const &result)
{
    return os << meta::get_group<0>(result);
}

namespace std
{
    template<meta::capture_storage CaptureStorage, typename NameSpec>
    struct tuple_size<meta::basic_regex_result<CaptureStorage, NameSpec>>
    {
        static constexpr size_t value = meta::basic_regex_result<CaptureStorage, NameSpec>::capture_count;
    };

    template<size_t ID, meta::capture_storage CaptureStorage, typename NameSpec>
    struct tuple_element<ID, meta::basic_regex_result<CaptureStorage, NameSpec>>
    {
        using type = tuple_element_t<ID + 1, CaptureStorage>;
    };
}
#endif //MREGEX_REGEX_RESULT_HPP