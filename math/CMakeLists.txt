# LIBRARY

set(
        HEADERS
        include/math/vec3.hpp
        include/math/mat3.hpp
        include/math/quat.hpp
        include/math/rand.hpp
)

set(
        SOURCES
        src/vec3.cpp
        src/mat3.cpp
        src/quat.cpp
        src/rand.cpp
)

add_library(math ${HEADERS} ${SOURCES})

target_include_directories(math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# TESTS

set(
        TESTS
        test/run.cpp
        test/math_test.cpp
)

add_executable(math_unittest ${TESTS})

target_link_libraries(math_unittest PUBLIC math gtest_main)