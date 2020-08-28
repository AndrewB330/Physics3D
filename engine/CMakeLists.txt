# LIBRARY

set(
        HEADERS
        include/engine/world.hpp
        include/engine/geometry/collision.hpp
        include/engine/physics/object.hpp
        include/engine/physics/material.hpp
        include/engine/physics/inertia.hpp
        include/engine/geometry/bounding_box.hpp
        include/engine/geometry/shape.hpp
        include/engine/geometry/utils.hpp
        include/engine/timer.hpp
        include/engine/geometry/transform.hpp
        include/engine/physics/constraints.hpp)

set(
        SOURCES
        src/geometry/bounding_box.cpp
        src/physics/material.cpp
        src/physics/object.cpp
        src/world.cpp
        src/geometry/collision.cpp
        src/timer.cpp
        src/geometry/shape.cpp
        src/geometry/utils.cpp
        src/physics/inertia.cpp
        src/geometry/transform.cpp include/engine/assert.hpp src/physics/constraint.cpp)

add_library(engine ${HEADERS} ${SOURCES})
target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(engine PUBLIC math)

# TESTS

set(
        TESTS
        test/main.cpp
        test/test_collision.cpp
        test/test_shape.cpp)

add_executable(engine_unittest ${TESTS})
target_link_libraries(engine_unittest PUBLIC engine gtest_main)