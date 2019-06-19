cmake_minimum_required(VERSION 3.10)

project(kitten_test_package LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(kitten REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            kitten::kitten
)

target_compile_features(${PROJECT_NAME}
        PRIVATE
            cxx_std_17
)
