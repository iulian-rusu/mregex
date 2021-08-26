#ifndef META_ITERABLE_GENERATOR_ADAPTER_HPP
#define META_ITERABLE_GENERATOR_ADAPTER_HPP

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
    struct iterable_generator_adapter : protected Gen
    {
        class iterator;

        using value_type = std::invoke_result_t<Gen>;
        using iterator_type = iterator;

        template<typename Func>
        constexpr explicit iterable_generator_adapter(Func &&func)
        noexcept(std::is_nothrow_move_constructible_v<Gen>)
            : Gen{std::forward<Func>(func)}
        {}

        /**
         * Lazy iterator over the generated result.
         */
        class iterator
        {
            iterable_generator_adapter<Gen> &gen;
            value_type current_result;
            bool active;
        public:
            template<typename Res>
            constexpr explicit iterator(iterable_generator_adapter<Gen> &g, Res &&res, bool a)
            noexcept(std::is_nothrow_move_constructible_v<Res>)
                : gen{g}, current_result{std::forward<Res>(res)}, active{a}
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
                current_result = std::move(gen());
                active = static_cast<bool>(current_result);
                return *this;
            }

            template<typename Iter>
            constexpr bool operator==(Iter const &rhs) const noexcept
            {
                return static_cast<bool>(*this) == static_cast<bool>(rhs);
            }

            template<typename Iter>
            constexpr bool operator!=(Iter const &rhs) const noexcept
            {
                return static_cast<bool>(*this) != static_cast<bool>(rhs);
            }
        };

        struct iteration_end_marker : std::false_type {};

        constexpr auto begin() noexcept
        {
            auto initial_result = (*this)();
            auto const initial_state = static_cast<bool>(initial_result);
            return iterator{*this, std::move(initial_result), initial_state};
        }

        constexpr auto end() const noexcept
        {
            return iteration_end_marker{};
        }
    };

    template<bool_testable_generator G>
    iterable_generator_adapter(G &&) -> iterable_generator_adapter<std::decay_t<G>>;
}
#endif //META_ITERABLE_GENERATOR_ADAPTER_HPP