# LIBRARY

set(
        HEADERS
        include/engine/world.hpp
        include/engine/physics/object.hpp
        include/engine/physics/material.hpp
        include/engine/physics/inertia.hpp
        include/engine/utils.hpp
        include/engine/timer.hpp
        include/engine/geometry/transform.hpp
        include/engine/physics/constraints.hpp include/engine/collision/collision_engine.hpp include/engine/collision/bounding_volume.hpp include/engine/collision/tree.hpp include/engine/physics/arbiter.hpp)

set(
        SOURCES
        src/collision/bounding_volume.cpp
        src/physics/material.cpp
        src/physics/object.cpp
        src/world.cpp
        src/timer.cpp
        src/utils.cpp
        src/physics/inertia.cpp
        src/geometry/transform.cpp include/engine/assert.hpp src/physics/constraint.cpp include/engine/collision/collider.hpp src/collision/collider.cpp src/collision/collision_engine.cpp src/collision/tree.cpp src/physics/arbiter.cpp)

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