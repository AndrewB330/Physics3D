# LIBRARY

set(
        HEADERS
        include/engine/world.hpp
        include/engine/collision.hpp
        include/engine/model/object.hpp
        include/engine/model/material.hpp
        include/engine/model/bounding_box.hpp
        include/engine/model/objects/polyhedron.hpp
        include/engine/model/objects/sphere.hpp
        include/engine/model/objects/box.hpp
        include/engine/utils.hpp
)

set(
        SOURCES
        src/box.cpp
        src/bounding_box.cpp
        src/material.cpp
        src/object.cpp
        src/polyhedron.cpp
        src/sphere.cpp
        src/world.cpp
        src/collision.cpp
        src/utils.cpp
)

add_library(engine ${HEADERS} ${SOURCES})
target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(engine PUBLIC math)

# TESTS

set(
        TESTS
        test/run.cpp
        test/collision_test.cpp
)

add_executable(engine_unittest ${TESTS})
target_link_libraries(engine_unittest PUBLIC engine gtest_main)