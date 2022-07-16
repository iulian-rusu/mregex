#ifndef MREGEX_INPUT_RANGE_ADAPTER_HPP
#define MREGEX_INPUT_RANGE_ADAPTER_HPP

#include <mregex/utility/concepts.hpp>
#include <mregex/utility/meta_helpers.hpp>

namespace meta
{
    /**
     * Adapter that allows iterating through the results returned by a generating function.
     * The generator is required to return a boolean-convertible value to signal
     * its iteration state. A false value implies the end of the iteration.
     *
     * @tparam Gen   The type of callable used to produce the desired data
     */
    template<bool_testable_generator Gen>
    struct input_range_adapter : private Gen
    {
        using result_type = forward_result_t<std::invoke_result_t<Gen>>;
        using value_type = std::remove_reference_t<result_type>;

        template<typename G>
        constexpr explicit input_range_adapter(G &&g)
        noexcept(std::is_nothrow_move_constructible_v<Gen> && noexcept(Gen::operator()()))
                : Gen{std::forward<G>(g)}, current_result{Gen::operator()()}
        {}

        struct iterator
        {
            using value_type = typename input_range_adapter<Gen>::value_type;
            using pointer = std::add_pointer_t<value_type>;
            using reference = std::add_lvalue_reference_t<value_type>;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;

            constexpr iterator() noexcept = default;

            constexpr iterator(input_range_adapter<Gen> *ptr) noexcept
                    : gen_ptr{ptr}
            {}

            constexpr explicit operator bool() const noexcept
            {
                return gen_ptr->active();
            }

            constexpr value_type &operator*() const noexcept
            {
                return gen_ptr->get();
            }

            constexpr value_type *operator->() const noexcept
            {
                return gen_ptr->get();
            }

            constexpr iterator &operator++() noexcept
            {
                gen_ptr->advance();
                return *this;
            }

            constexpr iterator operator++(int) noexcept
            {
                iterator old_iter{gen_ptr};
                this->operator++();
                return old_iter;
            }

            constexpr bool operator==(iterator) const noexcept
            {
                return !gen_ptr->active();
            }

            constexpr bool operator!=(iterator) const noexcept
            {
                return gen_ptr->active();
            }

        private:
            input_range_adapter<Gen> *gen_ptr = nullptr;
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
            return iterator{this};
        }

        /**
         * Returns a default constructed iterator which does not point to any object.
         * This iterator is used to invoke operator!= while iterating and should never
         * be dereferenced or incremented.
         *
         * @return  An empty end-of-range iterator
         */
        [[nodiscard]] constexpr auto end() const noexcept
        {
            return iterator{};
        }

        [[nodiscard]] constexpr bool active() const noexcept
        {
            return static_cast<bool>(current_result);
        }

        [[nodiscard]] constexpr auto &get() & noexcept
        {
            return current_result;
        }

        [[nodiscard]] constexpr auto const &get() const & noexcept
        {
            return current_result;
        }

        [[nodiscard]] constexpr auto &&get() && noexcept
        {
            return current_result;
        }

        constexpr void advance() noexcept
        {
            current_result = this->operator()();
        }

    private:
        result_type current_result;
    };

    template<bool_testable_generator G>
    input_range_adapter(G &&) -> input_range_adapter<std::remove_reference_t<G>>;
}
#endif //MREGEX_INPUT_RANGE_ADAPTER_HPP