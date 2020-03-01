project(kitten_tests LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}
        function_test.cpp
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
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME}
            PRIVATE
                /Wall /W4
    )
else()
    message("Unknown compiler..skipping configuration for warnings")
endif()

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

find_package(Catch2 REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            rvarago::kitten
            Catch2::Catch2
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
