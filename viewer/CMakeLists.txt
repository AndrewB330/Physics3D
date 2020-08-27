# LIBRARY

set(
        HEADERS
        include/viewer/controls.hpp
        include/viewer/drawing.hpp
        include/viewer/graphics.hpp
)

set(
        SOURCES
        src/controls.cpp
        src/drawing.cpp
        src/graphics.cpp
)

include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})

add_library(viewer ${HEADERS} ${SOURCES})
target_include_directories(viewer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(viewer PUBLIC engine ${GLUT_LIBRARIES} -lopengl32 -lglu32)
