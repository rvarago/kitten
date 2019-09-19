# kitten

This is a small header-only library with some machinery meant to extend the already great STL with some concepts obtained from Category Theory, such as functors and monads, in order to make the composition of some C++ type constructors even simpler.

It's basically a collection of utilities from my recent studies of Category Theory expressed in C++.

Category Theory -> Category -> Cat -> Kitten 🐈

[![Build Status](https://travis-ci.org/rvarago/absent.svg?branch=master)](https://travis-ci.org/rvarago/kitten)

## Problem description

Functors, applicatives, monads, etc are mathematical structures studied in Category Theory that capture the essence of composition. And given that programming is all about composition, they have plenty of practical applications, particularly in Pure Functional Programming.

Furthermore, they are structures that must respect mathematical laws that restrict their behavior. [learnyouahaskell](http://learnyouahaskell.com/functors-applicative-functors-and-monoids)
provides a very good explanation about these laws in terms of programming, but of course, there are many other
resources to learn about them.

Although they're fairly abstract concepts, there are many practical applications for them when we want to compose
_effectful functions_. We can think about an effectful function as a function `f` that instead of returning a value of type
`A` it returns a value of type `X<A>` where `X` represents an effect which models an "extra information" about the computation
done inside `f`.

For example:

- A function `f` that can fail to produce a value of type `A` might return an `std::optional<A>`
where `std::optional` is a piece of extra information (an effect) that models the absence of a value.
- A function `f` that can return multiple values of type `A` might return an `std::vector<A>` where
`std::vector` is a piece of "extra information" (an effect) that models multiple values.

When we have functions `f: A -> B` and `g: B -> C` we can compose these two functions into a new function
`h: A -> C` by using the usual function composition:

` h(x) = g(f(x)), A -> C`

Therefore, it eliminates the intermediate steps involving `B`.

That's only possible because of the co-domain `A` of `f` matches with the domain `B` of `g`, i.e.
the output of the first function can be fed as the input for the second function. But, what happens when we have effectful
functions involved?

_kitten_ has the goal of providing instances for some common abstractions from Category Theory to C++. Internally,
 it makes heavy-usage of the STL to extend the capabilities of supported C++ types.

### Functors

Now, consider `f` as an effectful function: `f: A -> X<B>`, where `X` models some effect. We can't compose
`g: B -> C` anymore, since it expects `B` and therefore can't be fed with an `X<B>`.

Given that we can't do the usual function composition, we need a more powerful way to do composition.

We need a functor.

If `X<T>` admits a functor for some type parameter `T`, we can compose `f` and `g` by using `fmap`:

`fmap(X<A>, w: A -> B): X<B>`

`fmap` receives a functor `X<A>`, a function `w: A -> B` that would do the composition of the types `A` and `B`, and
it returns a new functor `X<B>`. It basically:
 
1. unwraps `X<A>` into `A`
2. feeds `A` into `w`
3. wraps (or lifts) the output of `w` into an `X<B>`
4. then returns `X<B>`

Hence, we can do:

`fmap(f(), g)`

Using _kitten_, one example of using a functor is:

```
auto const maybe_name = maybe_find_person() | get_name; // or fmap(maybe_find_person(), get_name)
```

Where `maybe_find_person` returns an `std::optional<person>`, and then the wrapped object of type `person` is fed into
`get_name` that returns an object of type `name`.

Thus, the result of the whole composition is of type `std::optional<name>`.

### Applicatives

What happens if `f` takes several arguments? For instance, we have the objects `xa: X<A>` and `xb: X<B>` and the
function `f: (A, B) -> C`.
How can we combine two effects `xa` and `xb` via `f` to obtain `X<C>`?

If we use `fmap` as we did before, we wouldn't be able, because `fmap` accepts only one argument, and we need to somehow
provide two.

We need a structure that's more powerful than a functor, we need an applicative.

If `X<T>` admits an applicative for some type parameter `T`, we can compose `f` using `combine` (also called `liftA2`):

`combine(X<A>, X<B>, w: (A, B) -> C): X<C>`

`combine` receives the applicatives `X<A>` and `X<B>`, a binary function `w: (A, B) -> C` that would do the composition
of the types `A` and `B`, and it returns a new applicative `X<B>`. It basically:

1. unwraps `X<A>` into `A`
2. unwraps `X<B>` into `B`
3. feeds `A` and `B` into `w`
3. wraps the result `C` into `X<C>`
4. then returns `X<C>`

Hence, we can do:

`combine(xa, xb, f)`

Using _kitten_, one example of using an applicative is:

```
auto const maybe_three = maybe_one() + maybe_two(); // or combine(maybe_one(), maybe_two(), std::plus{})
```

Where `maybe_one` and `maybe_two` return instances of `std::optional<int>`, and then unwrapped objects of types
`int` and `int` are fed into operator `+` (that defaults to `std::plus{}` for the unwrapped types) that returns an object
of type `int` which is finally wrapped again in an `std::optional<int>`.

Thus, the result of the whole composition is of type `std::optional<int>`.

And what happens if we have n-ary rather than binary function `f`?

We can simply chaining operator `+`, like:

```
auto const maybe_ten = maybe_one() + maybe_two() + maybe_three() + maybe_four(); // and so on ...
```

And what should we do if we need a different operation instead of addition?

Given that operator `+` accepts two parameters, we have to use a convenient and general overload that accepts a tuple of two applicatives:

```
auto const maybe_six_as_string = std::tuple{maybe_two, maybe_three} + [](auto const& first, auto const& second) {
            return std::to_string(first * second);
};
```

### Monads

What happens if `f` and `g` are both effectul functions: `f: A -> X<B>` and `g: B -> X<C>`. How can
we compose `f` and `g`?

If we use `fmap` as we did before, we would end up with a return type `X<X<C>>`, that nests the same effect. This type
would then need to be flattened, or collapsed, into an `X<C>`, we need a structure that knows how to flat the effects.

We need a structure that's more powerful than a functor, we need a monad.

If `X<T>` admits a monad for some type parameter `T`, we can compose `f` and `g` by using `bind`:

`bind(X<A>, w: A -> X<B>): X<B>`

`bind` receives a monad `X<A>`, a function `w: A -> X<B>` that would do the composition of the types `A` and
`X<B>`, and it returns a new monad `X<B>`. It basically:
 
1. unwraps `X<A>` into `A`
2. feeds `A` into `w`
3. flats `X<X<B>>` into `X<B>`
4. then returns `X<B>`

Hence, we can do:

`bind(f(), g)`

Using _kitten_, one example of using a monad is:

```
auto const maybe_name = maybe_find_person() >> maybe_get_name; // or bind(maybe_find_person(), maybe_get_name)
```

Where `maybe_find_person` returns an `std::optional<person>`, and then the wrapped object of type `person` is fed into
`maybe_get_name` that itself returns an object of type `std::optional<name>`.

Thus, the result of the whole composition is of type `std::optional<name>`.

## Multi-functors

A multi-functor generalizes a functor in the sense that instead of having only 1 type parameter, it can have `N` different types.

Given a multi-functor of arity 2, also called bi-functor,`X<A1, B1>`, and the functions `fa: A1 -> A2` and `fb: B1-> B2`,
a multi-functor uses `multimap` to instantiate a new bi-functor `X<A2, B2>` via mapping the types through  `fa` and `fb`.

An interesting use case for a multi-functor is where we have a function that returns an `std::variant<A1, B1, C1>` and we
want to map such type to `std::variant<A2, B2, C2>` via several functions `fa: A1 -> A2`, `fb: B1 -> B2`, and `fc: C1 -> C2`
using _kitten_, we can do:

```
auto const variant_A2_B2_C2 = variant_A1_B1_C1 || syntax::overloaded {
    [](A1 value) { return A1_to_A2(value); },
    [](B1 value) { return B1_to_B2(value); },
    [](C1 value) { return C1_to_C2(value); }
};
```

Where `syntax::overloaded` is a helper function that enables us to create a function object on-the-fly, that receives a
set of lambda expressions, and the right overload is then selected at compile-time depending on the type held by the
`std::variant<A1, B1, C1>`.

## kitten

_kitten_ relies on the STL to provide functor, applicative, monad, and multi-functor instances for some C++ data types. Given that the data type admits
such instances, it's then possible to use the combinators available as free functions:

- `fmap` for types that have functor instances
- `combine` for types that have applicative instances
- `bind` for types that have monad instances
- `multimap` for types that have multi-functor instances

Also, to simplify notation, they also come as overloaded operators that enable a, hopefully, nicer, infix syntax:

- `|` as an alias for `fmap`
- `+` as an alias for `combine`
- `>>` as an alias for `bind`
- `||` as an alias for `multimap`

The combinators are available conveniently in the header: `kitten/kitten.h`, or by importing each one separately. And
the main namespace is `rvarago::kitten`.

Note that it's possible that a type may not admit instances for all the structures, e.g a type may have a functor but not a monad.

### Adapters

The following types are currently supported:

|         Type         | Functor | Applicative | Monad | Multi-functor |
|:--------------------:|:-------:|-------------|-------|:-------------:|
|    `either<A, E>`    |    x    |     x       | x     |               |
|  `std::optional<T>`  |    x    |     x       | x     |               |
|    `std::deque<T>`   |    x    |     x       | x     |               |
|    `std::list<T>`    |    x    |     x       | x     |               |
| `std::variant<T...>` |         |             |       |       x       |
|   `std::vector<T>`   |    x    |     x       |       |               |

- `either<A, E>` is a *left-biased* alias for `std::variant<A, E>`. And by left-biased, I mean that the mapping only
happens for the left type parameter `A`. For instance `fmap` receives a function `f: A -> B` and then
returns `either<B, E>`.

## Requirements

* C++17
* Make
* CMake
* Conan
* Docker

### Notes

- Make is only required if you want to use it to orchestrate task execution, for instance to invoke Conan, CMake, etc.
- Docker is only required if you want to run the tests inside a container.

## Build

The _Makefile_ wraps the commands to download dependencies (Conan), generate the build configuration, build, run the
unit tests, and clear the build folder. Please consult the Makefile to adapt
the commands in case you want to build _absent_ directly without using make.

* Compile:

``
make
``

By default, it also builds the unit tests, you can disable the behavior by:

``
make BUILD_TESTS=OFF
``


The build always assumes that the default profile (*profiles/common*) applies to your build. If that's not, then you
can specify your profile by setting _PROFILE_ as:

``
make PROFILE=<path_to_your_profile>
``

And to build with Release mode (by default it builds with Debug mode enabled):

``
make BUILD_TYPE=Release
``

* To run the unit tests, if previously compiled:

``
make test
``

### Run unit tests inside a Docker container

Optionally, it's also possible to run the unit tests inside a Docker container by executing:

``
make env-test
``

## Installing on the system

To install _kitten_:

``
sudo make install
``

Then, it's possible to import _kitten_ into external CMake projects, say in a target _myExample_, by simply adding the
following commands to its _CMakeLists.txt_:

``
find_package(kitten)
target_link_libraries(myExample rvarago::kitten)
``

## Packaging via Conan

To generate a package via Conan:

``
make conan-package
``

This will build the package _kitten_, run the test package, and then install it in the local Conan cache.
