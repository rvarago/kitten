from conans import ConanFile, CMake
import os

class KittenlsTestConan(ConanFile):
    name        = "kitten_test_package"
    author      = "Rafael Varago (rvarago)"

    generators  = "cmake_find_package"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        self.run(".{}{}".format(os.sep, self.name))
