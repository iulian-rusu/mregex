#ifndef MREGEX_UTILITY_INPUT_RANGE_ADAPTER_HPP
#define MREGEX_UTILITY_INPUT_RANGE_ADAPTER_HPP

#include <mregex/utility/concepts.hpp>
#include <mregex/utility/type_traits.hpp>

namespace meta
{
    /**
     * Adapter that allows iterating through the results returned by a generating function.
     * The generator is required to return a boolean-convertible value to signal the end of iteration.
     * The generator may be called once when constructing this object.
     *
     * @tparam Gen   The callable type used to generate the data
     */
    template<bool_testable_generator Gen>
    struct input_range_adapter : private Gen
    {
        using result_type = forward_result_t<std::invoke_result_t<Gen>>;
        using value_type = std::remove_reference_t<result_type>;

        template<typename G>
        constexpr explicit input_range_adapter(G &&generator)
        noexcept(std::is_nothrow_move_constructible_v<Gen> && noexcept(Gen::operator()()))
            : Gen{std::forward<G>(generator)}, _current_result{Gen::operator()()}
        {}

        struct iterator
        {
            using value_type = typename input_range_adapter<Gen>::value_type;
            using pointer = std::add_pointer_t<value_type>;
            using reference = std::add_lvalue_reference_t<value_type>;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;

            /**
             * Note: this default constructor is technically not required for the iterator
             * to satisfy `std::input_iterator`. However, due to a wrong implementation in some older versions of
             * the standard library (https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2325r3.html),
             * a default constructor is required to satisfy the `std::weakly_incrementable` concept.
             */
            constexpr iterator() noexcept : _target{nullptr} {}

            constexpr iterator(input_range_adapter<Gen> &generator) noexcept
                : _target{&generator}
            {}

            constexpr explicit operator bool() const noexcept
            {
                return _target->active();
            }

            constexpr value_type &operator*() const noexcept
            {
                return _target->get();
            }

            constexpr value_type *operator->() const noexcept
            {
                return &_target->get();
            }

            constexpr iterator &operator++() noexcept
            {
                _target->compute_next();
                return *this;
            }

            constexpr iterator operator++(int) noexcept
            {
                iterator old_iter{_target};
                this->operator++();
                return old_iter;
            }

            constexpr bool operator==(std::default_sentinel_t) const noexcept
            {
                return !_target->active();
            }

            constexpr bool operator!=(std::default_sentinel_t) const noexcept
            {
                return _target->active();
            }

        private:
            input_range_adapter<Gen> *_target;
        };

        /**
         * Returns an input iterator to the current element of the generator.
         * Calling this multiple times will usually result in different values
         * since the range provides input iterators only.
         *
         * @return  An input iterator pointing to the first element of the range
         */
        [[nodiscard]] constexpr auto begin() noexcept
        {
            return iterator{*this};
        }

        /**
         * Returns a sentinel object for the iterator of the range.
         * This sentinel can only be used to compare for equality with an iterator.
         *
         * @return  A sentinel object suitable for the iterator of the range
         */
        [[nodiscard]] constexpr auto end() const noexcept
        {
            return std::default_sentinel;
        }

        [[nodiscard]] constexpr bool active() const noexcept
        {
            return static_cast<bool>(_current_result);
        }

        [[nodiscard]] constexpr auto &get() & noexcept
        {
            return _current_result;
        }

        [[nodiscard]] constexpr auto const &get() const & noexcept
        {
            return _current_result;
        }

        [[nodiscard]] constexpr auto &&get() && noexcept
        {
            return _current_result;
        }

        [[nodiscard]] constexpr auto const &&get() const && noexcept
        {
            return _current_result;
        }

        constexpr void compute_next() noexcept
        {
            _current_result = Gen::operator()();
        }

    private:
        result_type _current_result;
    };

    template<bool_testable_generator G>
    input_range_adapter(G &&) -> input_range_adapter<std::remove_reference_t<G>>;
}
#endif //MREGEX_UTILITY_INPUT_RANGE_ADAPTER_HPP