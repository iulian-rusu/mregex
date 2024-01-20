#ifndef MREGEX_MATCH_RESULT_HPP
#define MREGEX_MATCH_RESULT_HPP

#include <iosfwd>
#include <optional>
#include <tuple>
#include <mregex/utility/tuple.hpp>
#include <mregex/regex_capture_storage.hpp>

namespace meta
{
    /**
     * Result of matching a regex.
     * Holds information about whether the match was successful and
     * allows accessing the content matched by capturing groups.
     *
     * @tparam CaptureStorage   The storage type used to hold the captures
     */
    template<capture_storage CaptureStorage>
    struct basic_match_result;

    /**
     * Result that holds views into the matched content.
     * The original input's lifetime must not expire before this object.
     *
     * @tparam Regex    The regex type used for matching
     * @tparam Iter     The forward iterator type used to view the matched content
     */
    template<typename Regex, std::forward_iterator Iter>
    using match_result_view = basic_match_result<regex_capture_view_storage<Regex, Iter>>;

    /**
     * Result that holds ownership of matched content.
     *
     * @tparam Regex    The regex type used for matching
     */
    template<typename Regex>
    using match_result = basic_match_result<regex_capture_storage<Regex>>;

    template<capture_storage CaptureStorage>
    struct basic_match_result
    {
        using capture_storage_type = CaptureStorage;
        using implicit_capture_type = std::tuple_element_t<0, capture_storage_type>;

        static constexpr bool is_view = is_capture_view<implicit_capture_type>;
        static constexpr std::size_t group_count = std::tuple_size_v<capture_storage_type>;

        constexpr basic_match_result() noexcept = default;

        template<capture_storage Captures>
        constexpr basic_match_result(Captures &&captures, bool matched)
        noexcept(std::is_nothrow_constructible_v<capture_storage_type, Captures &&>)
            : _captures{std::forward<Captures>(captures)}, _matched{matched}
        {}

        constexpr bool matched() const noexcept
        {
            return _matched;
        }

        constexpr auto length() const noexcept -> std::size_t
        {
            return std::get<0>(_captures).length();
        }

        constexpr auto begin() const noexcept
        {
            return std::get<0>(_captures).begin();
        }

        constexpr auto end() const noexcept
        {
            return std::get<0>(_captures).end();
        }

        /**
         * Performs a deep copy of all regex captures and returns
         * a new match result container that owns the memory.
         *
         * @return  A new match result object that holds ownership of captures
         */
        [[nodiscard]] auto as_memory_owner() const
        requires is_view
        {
            auto owned_captures = transform_groups([](auto const &capture) noexcept {
                return regex_capture{capture};
            });

            using owned_storage_type = std::remove_reference_t<decltype(owned_captures)>;
            return basic_match_result<owned_storage_type>{std::move(owned_captures), _matched};
        }

        /**
         * Converts the match result object to an instance of std::optional.
         * The returned optional will be empty if the regex did not match.
         *
         * @return A new instance of std::optional that contains the match result object
         */
        [[nodiscard]] constexpr auto as_optional() & noexcept(is_view)
        {
            return forward_self_as_optional(*this);
        }

        /**
         * Creates a new tuple by applying a function on each group of this match result.
         *
         * @param func  The function invoked to transform each capturing group
         * @return      A new std::tuple that contains the transformed groups
         */
        template<typename Func>
        [[nodiscard]] constexpr auto transform_groups(Func &&func) const
        {
            return transform_tuple(_captures, std::forward<Func>(func));
        }

        /**
         * Invokes a function for each group of this match result.
         *
         * @param func  The function invoked on each capturing group
         */
        template<typename Func>
        constexpr void for_each_group(Func &&func) &
        {
            iterate_tuple(_captures, std::forward<Func>(func));
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
        constexpr auto &group() & noexcept
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
        constexpr auto &group() & noexcept
        {
            return get_group_by_name<Name>(_captures);
        }

        /**
         * More overloads are required because C++20 doesn't allow deducing
         * the value category of "this" inside a method.
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

        template<typename Func>
        constexpr void for_each_group(Func &&func) const &
        {
            iterate_tuple(_captures, std::forward<Func>(func));
        }

        template<typename Func>
        constexpr void for_each_group(Func &&func) &&
        {
            iterate_tuple(std::move(_captures), std::forward<Func>(func));
        }

        template<typename Func>
        constexpr void for_each_group(Func &&func) const &&
        {
            iterate_tuple(std::move(_captures), std::forward<Func>(func));
        }

        template<std::size_t ID>
        constexpr auto const &group() const & noexcept
        {
            return get_group_by_index<ID>(_captures);
        }

        template<std::size_t ID>
        constexpr auto &&group() && noexcept
        {
            return get_group_by_index<ID>(std::move(_captures));
        }

        template<std::size_t ID>
        constexpr auto const &&group() const && noexcept
        {
            return get_group_by_index<ID>(std::move(_captures));
        }

        template<static_string Name>
        constexpr auto const &group() const & noexcept
        {
            return get_group_by_name<Name>(_captures);
        }

        template<static_string Name>
        constexpr auto &&group() && noexcept
        {
            return get_group_by_name<Name>(std::move(_captures));
        }

        template<static_string Name>
        constexpr auto const &&group() const && noexcept
        {
            return get_group_by_name<Name>(std::move(_captures));
        }

        /**
         * Tuple-like interface for structured binding decomposition.
         * Use the group() method for extracting captures.
         */
        template<std::size_t ID>
        constexpr auto &get() & noexcept
        {
            return get_group_by_index<ID>(_captures);
        }

        template<std::size_t ID>
        constexpr auto const &get() const & noexcept
        {
            return get_group_by_index<ID>(_captures);
        }

        template<std::size_t ID>
        constexpr auto &&get() && noexcept
        {
            return get_group_by_index<ID>(std::move(_captures));
        }

        template<std::size_t ID>
        constexpr auto const &&get() const && noexcept
        {
            return get_group_by_index<ID>(std::move(_captures));
        }

        constexpr bool operator==(bool value) const noexcept
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
            if (!self.matched())
                return std::nullopt;
            return std::optional{std::forward<Self>(self)};
        }

        template<std::size_t ID, capture_storage Captures>
        static constexpr decltype(auto) get_group_by_index(Captures &&captures) noexcept
        {
            static_assert(ID < group_count, "capturing group does not exist");
            return std::get<ID>(std::forward<Captures>(captures));
        }

        template<static_string Name, capture_storage Captures>
        static constexpr decltype(auto) get_group_by_name(Captures &&captures) noexcept
        {
            using capture_type = rename_capture_t<implicit_capture_type, symbol::name<Name>>;
            return std::get<capture_type>(std::forward<Captures>(captures));
        }

        capture_storage_type _captures;
        bool _matched{};
    };

    /**
     * Functors used to project a match result to one of its capturing groups.
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

template<meta::capture_storage CaptureStorage>
std::ostream &operator<<(std::ostream &os, meta::basic_match_result<CaptureStorage> const &result)
{
    return os << meta::get_group<0>(result);
}

template<meta::capture_storage CaptureStorage>
struct std::tuple_size<meta::basic_match_result<CaptureStorage>>
{
    static constexpr std::size_t value = meta::basic_match_result<CaptureStorage>::group_count;
};

template<size_t ID, meta::capture_storage CaptureStorage>
struct std::tuple_element<ID, meta::basic_match_result<CaptureStorage>>
{
    using type = std::tuple_element_t<ID, CaptureStorage>;
};
#endif //MREGEX_MATCH_RESULT_HPP