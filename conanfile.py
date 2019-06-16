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

    requires    = "gtest/1.8.0@bincrafters/stable"
    generators  = "cmake_find_package"

    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy("*.h")

    def package_id(self):
        self.info.header_only()
