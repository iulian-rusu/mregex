#ifndef CX_GENERATOR_H
#define CX_GENERATOR_H

#include "regex_result.h"

namespace cx
{
    /**
     * Generator that produces results based on a specified generating function.
     *
     * @tparam GeneratingFunction   The type of callable used to generated the desired data
     */
    template<typename GeneratingFunction>
    class generator : public GeneratingFunction
    {
    public:
        using result_t = std::invoke_result_t<GeneratingFunction>;

        template<typename Function>
        constexpr explicit generator(Function &&func)
        noexcept((std::is_nothrow_move_constructible_v<GeneratingFunction>))
                : GeneratingFunction(std::forward<Function>(func))
        {}

        /**
         * Lazy iterator over the generated results.
         * The results must be explicitly convertible to bool in order
         * to signal iteration ending.
         */
        class iterator
        {
            GeneratingFunction &gen_func;
            result_t current_result;
            bool has_finished;
        public:
            constexpr explicit iterator(GeneratingFunction &func, bool flag = false)
                    : gen_func(func), current_result(std::move(func())), has_finished(flag)
            {
            }

            constexpr explicit operator bool() const noexcept
            {
                return has_finished;
            }

            decltype(auto) operator*() noexcept
            {
                return current_result;
            }

            iterator &operator++() noexcept
            {
                current_result = std::move(gen_func());
                has_finished = !static_cast<bool>(current_result);
                return *this;
            }

            template<typename Iter>
            bool operator!=(const Iter &rhs) const noexcept
            {
                return static_cast<bool>(*this) != static_cast<bool>(rhs);
            }
        };

        struct end_iterator : std::bool_constant<true> {};

        auto begin() noexcept
        {
            return iterator{*this};
        }

        auto end() noexcept
        {
            return end_iterator{};
        }
    };

    template<typename Function>
    generator(Function &&) -> generator<std::decay_t<Function>>;
}
#endif //CX_GENERATOR_H
