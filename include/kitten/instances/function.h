#ifndef RVARAGO_KITTEN_FUNCTION_H
#define RVARAGO_KITTEN_FUNCTION_H

#include "kitten/functor.h"

#include <utility>

namespace rvarago::kitten {

    namespace types {

        template <typename Function>
        class function_wrapper {
            Function const f;
        public:
            explicit constexpr function_wrapper(Function invokable) noexcept : f{std::move(invokable)} {}

            template <typename... Args>
            constexpr auto operator()(Args&&... args) const noexcept -> decltype(f(std::forward<Args>(args)...)) {
                return f(std::forward<Args>(args)...);
            }
        };

        template <typename Function>
        constexpr function_wrapper<Function> fn(Function invokable) {
            return function_wrapper{std::move(invokable)};
        }

    }

    template <>
    struct functor<types::function_wrapper> {

        /**
         * Composes the functions first and second, in such a way that the result of the first feeds the second, i.e:
         *  fmap(first, second)(x) = second(fist(second))
         *
         * @param first a function A -> B to applied when the argument A are provided
         * @param second function B -> C to be applied with the result from previous application of first
         * @return a function composition A -> C that, when its argument is provided, applied first and then second
         */
        template <typename UnaryFunctionA, typename UnaryFunctionB>
        static constexpr decltype(auto) fmap(types::function_wrapper<UnaryFunctionA> const &first, types::function_wrapper<UnaryFunctionB> const& second) {
            return types::fn([first, second](auto&&... args) {
                return second(first(std::forward<decltype(args)>(args)...));
            });
        }

    };

    namespace traits {
        template <>
        struct is_functor<types::function_wrapper> : std::true_type{};
    }

}

#endif
