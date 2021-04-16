set(
        HEADERS
        include/application/application.hpp
        include/application/window.hpp
        include/application/window_listener.hpp
        include/application/window_renderer.hpp
)

set(
        SOURCES
        src/application/application.cpp
        src/application/window.cpp
)

add_library(application ${HEADERS} ${SOURCES})
target_include_directories(application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(application PUBLIC -lopengl32 -lglu32 -lglew32 ${SDL2_LIBRARIES} common)