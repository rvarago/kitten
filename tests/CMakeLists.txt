project(kitten_tests LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}
        either_test.cpp
        optional_test.cpp
        main.cpp
        sequence_container_test.cpp
        variant_test.cpp
)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME}
            PRIVATE
                -Wall -Wextra -Werror -ansi -pedantic
    )
else()
    target_compile_options(${PROJECT_NAME}
            PRIVATE
                /Wall /W4
    )
endif()

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

find_package(gtest REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            gtest::gtest
            rvarago::kitten
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
