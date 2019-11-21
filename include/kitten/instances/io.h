#ifndef RVARAGO_KITTEN_IO_H
#define RVARAGO_KITTEN_IO_H

#include <variant>

#include "kitten/applicative.h"
#include "kitten/functor.h"
#include "kitten/monad.h"

#include "kitten/detail/deriving/from_monad/derive_applicative.h"
#include "kitten/detail/deriving/from_monad/derive_functor.h"

namespace rvarago::kitten {

namespace types {

template <typename Action> class IO {
  Action action_;

public:
  explicit constexpr IO(Action action) : action_{action} {}

  constexpr auto run() const -> decltype(action_()) { return action_(); }

  constexpr auto run() -> decltype(action_()) { return action_(); }
};

using nothing = std::monostate;

template <typename Function> decltype(auto) action(Function invokable) {
  return [invokable](auto &&...) {
    invokable();
    return nothing{};
  };
}

} // namespace types

template <> struct monad<types::IO> {

  template <typename UnaryFunction, typename Action>
  static constexpr decltype(auto) bind(types::IO<Action> const &input,
                                       UnaryFunction f) {
    return types::IO{[input, f] { return f(input.run()).run(); }};
  }

  template <typename A> static constexpr auto wrap(A &&value) {
    return types::IO{[value] { return value; }};
  }
};

template <> struct applicative<types::IO> {

  template <typename BinaryFunction, typename A, typename B>
  static constexpr auto combine(types::IO<A> const &first,
                                types::IO<B> const &second, BinaryFunction f)
      -> types::IO<decltype(f(std::declval<A>(), std::declval<B>()))> {
    return detail::deriving::combine(first, second, f);
  }

  template <typename A> static constexpr auto pure(A &&value) -> types::IO<A> {
    return detail::deriving::wrap<types::IO>(std::forward<A>(value));
  }
};

template <> struct functor<types::IO> {

  template <typename UnaryFunction, typename A>
  static constexpr auto
  fmap(types::IO<A> const &input,
       UnaryFunction f) { //-> types::IO<decltype(f(std::declval<A>()))> {
    return detail::deriving::fmap(input, f);
  }
};

namespace traits {
template <> struct is_monad<types::IO> : std::true_type {};

template <> struct is_applicative<types::IO> : std::true_type {};

template <> struct is_functor<types::IO> : std::true_type {};
} // namespace traits

} // namespace rvarago::kitten

#endif
