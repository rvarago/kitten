from conans import ConanFile

class KittenConan(ConanFile):
    name        = "kitten"

    description = "A simple library that introduces monad-like types to simplify composition for the modern C++ programmer"
    author      = "Rafael Varago (rvarago)"
    topics      = ("composition", "monads", "category-theory", "declarative-programming")
    license     = "MIT"
    homepage    = "https://github.com/rvarago/kitten"
    url         = homepage
    exports 	= ["README.md", "LICENSE"]

    build_requires  = "catch2/2.11.0"
    generators      = "cmake_find_package"

    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy("*.h")

    def package_id(self):
        self.info.header_only()
