#ifndef CX_GENERATOR_H
#define CX_GENERATOR_H

#include "regex_result.h"
#include "iterator.h"

namespace cx
{
    /**
     * Gen that produces results based on a specified generating function.
     *
     * @tparam Prod   The type of callable used to produce the desired data
     */
    template<producer Prod>
    struct generator : Prod
    {
        template<typename Func>
        constexpr explicit generator(Func &&func) noexcept(std::is_nothrow_move_constructible_v<Prod>)
                : Prod(std::forward<Func>(func))
        {}

        auto begin() noexcept
        {
            auto initial_result = (*this)();
            auto initial_state = static_cast<bool>(initial_result);
            return lazy_iterator{*this, std::move(initial_result), initial_state};
        }

        auto end() noexcept
        {
            return end_iterator{};
        }
    };

    template<typename Func>
    generator(Func &&) -> generator<std::decay_t<Func>>;
}
#endif //CX_GENERATOR_H
