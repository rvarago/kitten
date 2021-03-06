#ifndef RVARAGO_KITTEN_DERIVE_FUNCTOR_H
#define RVARAGO_KITTEN_DERIVE_FUNCTOR_H

#include "kitten/functor.h"
#include "kitten/monad.h"

namespace rvarago::kitten::detail::deriving {

template <template <typename...> typename M, typename A, typename UnaryFunction>
constexpr decltype(auto) fmap(M<A> const &input, UnaryFunction f) {
    using MonadT = monad<M>;
    return MonadT::bind(input, [&f](auto const &value) { return MonadT::wrap(f(value)); });
}

}

#endif
