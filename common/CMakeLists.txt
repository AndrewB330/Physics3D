# LIBRARY

set(
        HEADERS
        include/common/math/vec3.hpp
        include/common/math/mat3.hpp
        include/common/math/quat.hpp
        include/common/math/rand.hpp
        include/common/math/vec2.hpp include/common/transformable.hpp)

set(
        SOURCES
        src/vec3.cpp
        src/mat3.cpp
        src/quat.cpp
        src/rand.cpp
        src/vec2.cpp)

add_library(common ${HEADERS} ${SOURCES})

target_include_directories(common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# TESTS

set(
        TESTS
        test/run.cpp
        test/math_test.cpp
)

add_executable(common_unittest ${TESTS})

target_link_libraries(common_unittest PUBLIC common gtest_main)