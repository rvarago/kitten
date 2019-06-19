project(kitten_tests LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}
        optional_test.cpp
        sequence_container_test.cpp
        main.cpp
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

find_package(gtest REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            gtest::gtest
            rvarago::kitten
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
