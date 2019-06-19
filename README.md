# kitten

A simple library that introduces monad-like types to simplify composition for the modern C++ programmer

## Problem description

Functors and monads are mathematical entities studied in Category Theory and they have plenty of applications in
Software Engineering, particularly in Functional Programming (FP). Moreover, they are structures that must respect some
laws in order to valid true functors and monads, [learnyouahaskell](http://learnyouahaskell.com/functors-applicative-functors-and-monoids)
provides a very good explanation about such laws in terms of programming languages, but of course, there are many other
sources to learn about them.

Although they're fairly abstract concepts, there are many practical applications for them when we want to compose
_effectful functions_. We can think about an effectful function as a function ``f`` that instead of returning a value of type
``A`` it returns a value of type ``X<A>`` where ``X`` represents an effect which models an "extra information" about the computation
done inside ``f``.

For example:

- A function ``f`` that can fail to produce a value of type ``A`` might return an ``std::optional<A>``
where ``std::optional`` is an effect that models the absence of a value.
- A function ``f`` that can return multiple values of type ``A`` might return an ``std::vector<A>`` where
``std::vector`` is an effect that models multiple values.

When we have functions ``f: A -> B`` and ``g: B -> C`` we can compose these two functions into a new functions
``h: A -> C`` by using the usual function composition:

`` h(x) = g(f(x)), A -> C``

Therefore, it eliminates the intermediate steps involving ``B``.

That's only possible because the co-domain ``A`` of ``f`` matches with the domain ``B`` of ``g``, i.e.
the output of the first function can go into the input of the second function. But What happens when we have effectful
functions involved?

### Functors

Now, consider ``f`` as an effectful function: ``f: A -> X<B>``, where ``X`` models some effect. We can't compose it
``g: B -> C`` anymore, since it expects ``B`` and can't be fed with an ``X<B>``.

Given that we can't do the usual function composition, we need a more powerful way to do composition.

We need a functor.

If ``X<T>`` admits a functor for some type parameter ``T``, we can compose ``f`` and ``g`` by using ``fmap``:

``fmap(X<A>, w: A -> B): X<B>``

``fmap`` receives a functor ``X<A>``, a function that would do the composition with the "raw" types" ``A`` and ``B``, and
it returns a new functor ``X<B>``. It basically unwraps ``X<A>``, feeds ``A`` into ``w``, wraps the output of ``w`` into
an ``X<B>``, and then finally returns it.

To do, we could do:

``fmap(f(), g)``

### Monads

What happens if the not only ``f`` is an effectul function ``f: A -> X<B>``, but also ``g`` is ``g: B -> X<C>``. How can
we compose ``f`` and ``g``?

If we use ``fmap`` as we did before, we would end up with a return type ``X<X<B>>``, that nests the same effect. This type
would then need to be flattened, or collapsed, into an ``X<B>``, we need a structure that knows how to flat the effects.

We need a structure more powerful than a functor, we need a monad.

If ``X<T>`` admits a monad for some type parameter ``T``, we can compose ``f`` and ``g`` by using ``bind``:

``bind(X<A>, w: A -> X<B>): X<B>``

``bind`` receives a monad ``X<A>``, a function that would do the composition with the types ``A`` and
``X<B>``, and it returns a new monad ``X<B>``. It basically unwraps ``X<A>``, feeds ``A`` into ``w``, and returns the
output of ``w`` which is already wrapped inside a monad, do any necessary flattening before.

To do, we could do:

``bind(f(), g)``

## kitten

_kitten_ relies on the STL to provide functor and monad instances for some C++ data types. Given that the data type admits
such instances, it's then possible to use the combinators available as free functions:

- ``fmap``
- ``bind``

Also, to simplify notation, they also come as overloaded operators that enable a, hopefully, nicer, infix syntax:

- ``|`` as an alias for ``fmap``
- ``>>`` as an alias for ``bind``

The combinators are available conveniently in the header: ``kitten/kitten.h``, or by importing each one separately. And
the main namespace is ``rvarago::kitten``.

### Adapters

The following types are currently supported as both functors and monads:

- ``std::optional<T>``
- ``std::deque<T>``
- ``std::list<T>``
- ``std::vector<T>``
- ``either<A, E>``: a *left-biased* alias for ``std::variant<A, E>``. And by left-biased, I mean that the mapping only
happens for the left type parameter ``A``, for instance ``fmap`` receives a function `f: A -> B` and then
returns ``either<B, E>``

## Requirements

* C++17
* Make
* CMake
* Conan
* Docker

Note that Docker is only required if you want to run the tests inside a container. In that case, you just need to have Make,
since all the other requirements are shipped inside the docker image. Therefore for quick exploration and collaboration,


## Build

The _Makefile_ wraps the commands to download dependencies (Conan), generate the build configuration, build, run the
unit tests, and clear the build folder.

* Compile (by default, it also compiles the unit tests):

```
make
```

By default, it also builds the unit tests, you can disable the behavior by:

```
make WITH_TESTS=false
```


The build always assumes that the default profile (*profiles/common*) applies to your build. If that's not, then you
can specify your profile by setting _PROFILE_ as:

```
make PROFILE=<path_to_your_profile>
```

* To run the unit tests:

```
make test
```

### Run unit tests inside a Docker container

Optionally, it's also possible to run the unit tests inside a Docker container by executing:

```
make env-test
```

## Installing on the system

To install _kitten_:

```
sudo make install
```

Then, it's possible to import _kitten_ into external CMake projects, say in a target _myExample_, by simply adding the
following commands to its _CMakeLists.txt_:

```
find_package(kitten)
target_link_libraries(myExample rvarago::kitten)
```

## Packaging via Conan

To generate a package via Conan:

```
make conan-package
```

This will build the package _kitten_, run the test package, and then install it in the local Conan cache.