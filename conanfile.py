from conans import ConanFile

class KittenConan(ConanFile):

    build_requires  = "catch2/2.11.0"
    generators      = "cmake_find_package"
