#include "ctr.h"

int main()
{
    using namespace ctr;

    static_assert(std::is_same_v<typename ctr::parser<"ab+">::ast, sequence<character<'a'>, plus<character<'b'>>>>);
    static_assert(std::is_same_v<typename ctr::parser<"\\(?">::ast, optional<character<'('>>>);
    static_assert(std::is_same_v<
        typename ctr::parser<"((tuv)?b+)*|x">::ast,
        alternation
        <
            star
            <
                sequence
                <
                    optional
                    <
                        sequence
                        <
                            character<'t'>,
                            character<'u'>,
                            character<'v'>
                        >
                    >,
                    plus<character<'b'>>
                >
            >,
            character<'x'>
        >
    >);
}
