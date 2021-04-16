set(
        HEADERS
        src/debug_ui.hpp
        src/game_renderer.hpp
        src/controls/camera.hpp
        src/controls/controls.hpp
        src/pipeline_gl/pipeline_gl.hpp
        src/game/voxel/voxel_object.hpp
        src/game/voxel/voxel_world.hpp
        src/game/voxel/voxel_texture_manager.hpp
        src/algorithms/fenwick_tree.hpp)

set(
        SOURCES
        src/runnable.cpp
        src/debug_ui.cpp
        src/game_renderer.cpp
        src/controls/camera.cpp
        src/controls/controls.cpp
        src/pipeline_gl/pipeline_gl.cpp
        src/game/voxel/voxel_object.cpp
        src/game/voxel/voxel_world.cpp
        src/game/voxel/voxel_texture_manager.cpp
        src/algorithms/fenwick_tree.cpp)

add_executable(preview ${HEADERS} ${SOURCES})
target_include_directories(preview PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(preview PUBLIC application imgui math)