#ifndef CX_CAPTURE_BUILDER_H
#define CX_CAPTURE_BUILDER_H

/**
 * #include <type_traits>
#include <iostream>

template<typename First, typename ... Rest>
struct sum_vals
{
    static constexpr std::size_t capture_count = First::capture_count + sum_vals<Rest ...>::capture_count;
};

template<typename First>
struct sum_vals<First>
{
    static constexpr std::size_t capture_count = First::capture_count;
};

struct elem
{
    static constexpr auto capture_count = 0;
};

template<auto ID, typename inner = elem>
struct cap
 {
    static constexpr auto capture_count = 1 + inner::capture_count;
};

template<typename ... Elems>
struct seq
{
    static constexpr auto capture_count = sum_vals<Elems ...>::capture_count;
};

template<typename T>
struct star
{
    static constexpr auto capture_count = T::capture_count;
};



template<auto ID, typename Wrapper>
struct renumber_captures
{
    using type = Wrapper;
};

template<auto ID, typename Inner, template<typename ...> typename Wrapper>
struct renumber_captures<ID, Wrapper<Inner>>
{
    using type = typename renumber_captures<ID, Inner>::type;
};

template<auto ID, typename Wrapper>
using renumber_captures_t = typename renumber_captures<ID, Wrapper>::type;

template<auto ID, typename ... Rest>
struct renumber_captures<ID, seq<Rest ...>>
{
    using type = seq<renumber_captures_t<ID, Rest> ...>;
};

template<auto ID, auto N, typename Inner>
struct renumber_captures<ID, cap<N, Inner>>
{
    static constexpr auto capture_count = Inner::capture_count;
    using type = cap<N + ID - capture_count, typename renumber_captures<ID + 1, Inner>::type>;
};

template<typename>
struct ident;

int main()
{
    using T1 = seq<star<cap<1>>, star<cap<4, cap<3, cap<2>>>>, cap<5>, cap<6>, cap<8,
    cap<7>>>;
    //ident<T1>{};
    ident<renumber_captures_t<0, T1>> {};
}

 */

#include "ast.h"

namespace cx
{

    template<auto Offset, typename Wrapper>
    struct renumber_captures
    {
        using type = Wrapper;
    };

    template<auto Offset, typename Wrapper>
    using renumber_captures_t = typename renumber_captures<Offset, Wrapper>::type;

    template<auto Offset, template<typename ...> typename Wrapper,  typename ... Inner>
    struct renumber_captures<Offset, Wrapper<Inner ...>>
    {
        using type = Wrapper<renumber_captures_t<Offset, Inner> ...>;
    };

    template<auto Offset, typename ... Rest>
    struct renumber_captures<Offset, sequence<Rest ...>>
    {
        using type = sequence<renumber_captures_t<Offset, Rest> ...>;
    };

    template<auto Offset, auto N, typename Inner>
    struct renumber_captures<Offset, capturing<N, Inner>>
    {
        static constexpr auto capture_count = Inner::capture_count;
        using type = capturing<N + Offset - capture_count, renumber_captures_t<Offset + 1, Inner>>;
    };
}
#endif //CX_CAPTURE_BUILDER_H
