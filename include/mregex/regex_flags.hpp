#ifndef MREGEX_REGEX_FLAGS_HPP
#define MREGEX_REGEX_FLAGS_HPP

namespace meta
{
    /**
     * Type used to store regex flags as a bitfield.
     *
     * @tparam ID   The unique identifier of the flag
     */
    template<std::size_t ID>
    struct regex_flags : std::integral_constant<std::size_t, ID>{};

    namespace flag
    {
        /**
         * Indicates the absence of flags.
         */
        inline constexpr auto none = regex_flags<0x0>{};

        /**
         * Makes matching case-insensitive.
         */
        inline constexpr auto icase = regex_flags<0x1>{};
        inline constexpr auto i = icase;

        /**
         * Makes wildcards (.) match newline characters.
         */
        inline constexpr auto dotall = regex_flags<0x2>{};
        inline constexpr auto s = dotall;

        /**
         * Makes anchors ($ and ^) also match beginnings/ends of lines.
         */
        inline constexpr auto multiline = regex_flags<0x4>{};
        inline constexpr auto m = multiline;

        /**
         * Makes the Kleene star operator (*) consume as few elements as possible.
         */
        inline constexpr auto ungreedy = regex_flags<0x8>{};
        inline constexpr auto U = ungreedy;

        /**
         * Makes the compiler unroll even non-trivial repetition loops.
         */
        inline constexpr auto unroll = regex_flags<0x10>{};
        inline constexpr auto r = unroll;
    }

    template<std::size_t N, std::size_t M>
    constexpr bool operator&(regex_flags<N>, regex_flags<M>) noexcept
    {
        return N & M;
    }

    template<std::size_t N, std::size_t M>
    constexpr regex_flags<N | M> operator|(regex_flags<N>, regex_flags<M>) noexcept
    {
        return {};
    }
}
#endif //MREGEX_REGEX_FLAGS_HPP