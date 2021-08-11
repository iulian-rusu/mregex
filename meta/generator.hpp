#ifndef META_GENERATOR_HPP
#define META_GENERATOR_HPP

namespace meta
{
    /**
     * Generator that produces results based on a specified generating function.
     *
     * @tparam Prod   The type of callable used to produce the desired data
     */
    template<producer Prod>
    struct generator : Prod
    {
        using value_type = std::invoke_result_t<Prod>;
        
        template<typename Func>
        constexpr explicit generator(Func &&func)
        noexcept(std::is_nothrow_move_constructible_v<Prod>)
                : Prod{std::forward<Func>(func)}
        {}

        /**
         * Lazy iterator over the generated results.
         * A result that is explicitly convertible to the boolean false
         * signals iteration ending.
         */
        class iterator
        {
            generator<Prod> &gen;
            value_type current_result;
            bool active;
        public:
            template<typename Res>
            constexpr explicit iterator(generator<Prod> &g, Res &&res, bool a)
            noexcept(std::is_nothrow_move_constructible_v<Res>)
                    : gen{g}, current_result{std::forward<Res>(res)}, active{a}
            {}

            constexpr explicit operator bool() const noexcept
            {
                return active;
            }

            value_type &operator*() noexcept
            {
                return current_result;
            }

            iterator &operator++() noexcept
            {
                current_result = std::move(gen());
                active = static_cast<bool>(current_result);
                return *this;
            }

            template<typename Iter>
            bool operator==(Iter const &rhs) const noexcept
            {
                return static_cast<bool>(*this) == static_cast<bool>(rhs);
            }

            template<typename Iter>
            bool operator!=(Iter const &rhs) const noexcept
            {
                return static_cast<bool>(*this) != static_cast<bool>(rhs);
            }
        };

        struct end_iterator : std::bool_constant<false> {};

        auto begin() noexcept
        {
            auto initial_result = (*this)();
            auto initial_state = static_cast<bool>(initial_result);
            return iterator{*this, std::move(initial_result), initial_state};
        }

        auto end() const noexcept
        {
            return end_iterator{};
        }
    };

    template<typename Func>
    generator(Func &&) -> generator<std::decay_t<Func>>;
}
#endif //META_GENERATOR_HPP