#ifndef RVARAGO_KITTEN_DERIVE_APPLICATIVE_H
#define RVARAGO_KITTEN_DERIVE_APPLICATIVE_H

#include "kitten/applicative.h"
#include "kitten/monad.h"

namespace rvarago::kitten::detail::deriving {

    template <typename BinaryFunction, template <typename ...> typename M, typename A, typename B, typename... Rest>
    constexpr auto combine(M<A, Rest...> const &first, M<B, Rest...> const& second, BinaryFunction f) -> M<decltype(f(std::declval<A>(), std::declval<B>()))> {
        using MonadT = monad<M>;
        return MonadT::bind(first, [&second, f](auto const& first_value) {
            return MonadT::bind(second, [&first_value, f](auto const& second_value) {
                return MonadT::wrap(f(first_value, second_value));
            });
        });
    }

}

#endif
