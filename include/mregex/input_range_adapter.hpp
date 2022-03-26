#ifndef MREGEX_INPUT_RANGE_ADAPTER_HPP
#define MREGEX_INPUT_RANGE_ADAPTER_HPP

#include <mregex/utility/concepts.hpp>

namespace meta
{
    /**
     * Adapter that allows iterating through the results returned by a generating function.
     * The generator is required to return a boolean-convertible value to signal
     * iteration ending.
     *
     * @tparam Gen   The type of callable used to produce the desired data
     */
    template<bool_testable_generator Gen>
    struct input_range_adapter : private Gen
    {
        template<typename G>
        constexpr explicit input_range_adapter(G &&g)
        noexcept(std::is_nothrow_move_constructible_v<Gen>)
                : Gen{std::forward<G>(g)}
        {}

        struct iteration_end_sentinel {};

        /**
         * Lazy iterator over the generated results.
         */
        struct iterator
        {
            using value_type = std::invoke_result_t<Gen>;
            using pointer = std::add_pointer_t<value_type>;
            using reference = std::add_lvalue_reference_t<value_type>;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;

            constexpr iterator() noexcept = default;

            template<typename Res>
            constexpr iterator(input_range_adapter<Gen> *ptr, Res &&res, bool a)
            noexcept(std::is_nothrow_move_constructible_v<Res>)
                    : gen_ptr{ptr}, current_result{std::forward<Res>(res)}, active{a}
            {}

            constexpr explicit operator bool() const noexcept
            {
                return active;
            }

            constexpr value_type const &operator*() const noexcept
            {
                return current_result;
            }

            constexpr value_type &operator*() noexcept
            {
                return current_result;
            }

            constexpr value_type *operator->() noexcept
            {
                return &current_result;
            }

            constexpr iterator &operator++() noexcept
            {
                current_result = std::move((*gen_ptr)());
                active = static_cast<bool>(current_result);
                return *this;
            }

            constexpr iterator operator++(int) noexcept
            {
                iterator old_iter{gen_ptr, std::move(current_result), active};
                ++(*this);
                return old_iter;
            }

            constexpr bool operator==(iteration_end_sentinel) const noexcept
            {
                return !active;
            }

            constexpr bool operator!=(iteration_end_sentinel) const noexcept
            {
                return active;
            }

        private:
            input_range_adapter<Gen> *gen_ptr = nullptr;
            value_type current_result{};
            bool active{};
        };

        /**
         * Returns an input iterator to the current element of the generator.
         * Calling this multiple times will usually result in different values
         * since the range provides input iterators only.
         *
         * @return  An input iterator pointing to the beginning of the range
         */
        [[nodiscard]] constexpr auto begin() noexcept
        {
            auto initial_result = (*this)();
            auto const initial_state = static_cast<bool>(initial_result);
            return iterator{this, std::move(initial_result), initial_state};
        }

        /**
         * Returns a sentinel used to verify if the generating functor has reached
         * its end state.
         *
         * @return A special sentinel type used to check the iterator state
         */
        [[nodiscard]] constexpr auto end() const noexcept
        {
            return iteration_end_sentinel{};
        }
    };

    template<bool_testable_generator G>
    input_range_adapter(G && ) -> input_range_adapter<std::decay_t<G>>;
}
#endif //MREGEX_INPUT_RANGE_ADAPTER_HPP