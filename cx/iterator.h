#ifndef CX_ITERATOR_H
#define CX_ITERATOR_H

namespace cx
{
    /**
     * Lazy iterator over the generated results.
     * A result that is explicitly convertible to the boolean false
     * signals iteration ending.
     *
     * @tparam Gen   The generator type to be iterated over
     */
    template<typename Gen>
    struct lazy_iterator
    {
    private:
        Gen &generator;
        std::invoke_result_t<Gen> current_result;
        bool active;
    public:
        template<typename Res>
        constexpr explicit lazy_iterator(Gen &g, Res &&res, bool a)
                : generator(g), current_result(std::forward<Res>(res)), active(a)
        {}

        constexpr explicit operator bool() const noexcept
        {
            return active;
        }

        decltype(auto) operator*() noexcept
        {
            return current_result;
        }

        lazy_iterator &operator++() noexcept
        {
            current_result = std::move(generator());
            active = static_cast<bool>(current_result);
            return *this;
        }

        template<typename Iter>
        bool operator!=(Iter const &rhs) const noexcept
        {
            return static_cast<bool>(*this) != static_cast<bool>(rhs);
        }
    };

    struct end_iterator : std::bool_constant<false>
    {};
}
#endif //CX_ITERATOR_H
