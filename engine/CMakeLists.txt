# LIBRARY

set(
        HEADERS
        include/engine/physics/physics_engine.hpp
        include/engine/physics/object.hpp
        include/engine/physics/material.hpp
        include/engine/physics/inertia.hpp
        include/engine/utils.hpp
        ../common/include/common/timer.hpp
        include/engine/collision/collision_engine.hpp include/engine/collision/bounding_volume.hpp include/engine/collision/tree.hpp include/engine/collision/shape.hpp include/engine/physics/constraint.hpp)

set(
        SOURCES
        src/collision/bounding_volume.cpp
        src/physics/material.cpp
        src/physics/object.cpp
        src/physics/physics_engine.cpp
        ../common/src/timer.cpp
        src/utils.cpp
        src/physics/inertia.cpp
        ../common/include/common/assert.hpp include/engine/collision/collider.hpp src/collision/collider.cpp src/collision/collision_engine.cpp src/collision/tree.cpp src/collision/shape.cpp src/physics/constraint.cpp include/engine/physics/arbiter.hpp src/physics/arbiter.cpp)

add_library(engine ${HEADERS} ${SOURCES})
target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(engine PUBLIC common)

# TESTS

set(
        TESTS
        test/main.cpp
        test/test_collision.cpp
        test/test_shape.cpp)

add_executable(engine_unittest ${TESTS})
target_link_libraries(engine_unittest PUBLIC engine gtest_main)