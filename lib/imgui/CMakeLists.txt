set(
        HEADERS
        include/imgui/imconfig.h
        include/imgui/imgui.h
        include/imgui/imgui_internal.h
        include/imgui/imstb_rectpack.h
        include/imgui/imstb_textedit.h
        include/imgui/imstb_truetype.h
        include/imgui/imgui_impl_sdl.h
        include/imgui/imgui_impl_opengl2.h
)

set(
        SOURCES
        src/imgui/imgui.cpp
        src/imgui/imgui_impl_sdl.cpp
        src/imgui/imgui_impl_opengl2.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_draw.cpp
)

add_library(imgui ${HEADERS} ${SOURCES})
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(imgui PUBLIC -lopengl32 -lglu32 -lglew32 ${SDL2_LIBRARIES})