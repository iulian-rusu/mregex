#ifndef META_RANGE_ADAPTER_HPP
#define META_RANGE_ADAPTER_HPP

#include "utility/concepts.hpp"

namespace meta
{
    /**
     * Adapter that allows iterating through the results returned by a generator.
     * The generator is required to return a boolean-convertible value to signal
     * iteration ending.
     *
     * @tparam Gen   The type of callable used to produce the desired data
     */
    template<bool_testable_generator Gen>
    struct range_adapter : protected Gen
    {
        template<typename F>
        constexpr explicit range_adapter(F &&f)
        noexcept(std::is_nothrow_move_constructible_v<Gen>)
                : Gen{std::forward<F>(f)}
        {}

        struct iteration_end_sentinel {};

        /**
         * Lazy iterator over the generated result.
         */
        struct iterator
        {
            using value_type = std::invoke_result_t<Gen>;
            using difference_type = std::size_t;
            using iterator_category = std::forward_iterator_tag;

            template<typename Res>
            constexpr explicit iterator(range_adapter<Gen> &gen, Res &&res, bool a)
            noexcept(std::is_nothrow_move_constructible_v<Res>)
                    : generator{gen}, current_result{std::forward<Res>(res)}, active{a}
            {}

            constexpr explicit operator bool() const noexcept
            {
                return active;
            }

            constexpr value_type &operator*() noexcept
            {
                return current_result;
            }

            constexpr iterator &operator++() noexcept
            {
                current_result = std::move(generator());
                active = static_cast<bool>(current_result);
                return *this;
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
            range_adapter<Gen> &generator;
            value_type current_result;
            bool active;
        };

        constexpr auto begin() noexcept
        {
            auto initial_result = (*this)();
            auto const initial_state = static_cast<bool>(initial_result);
            return iterator{*this, std::move(initial_result), initial_state};
        }

        constexpr auto end() const noexcept
        {
            return iteration_end_sentinel{};
        }
    };

    template<bool_testable_generator G>
    range_adapter(G &&) -> range_adapter<std::decay_t<G>>;
}
#endif //META_RANGE_ADAPTER_HPP