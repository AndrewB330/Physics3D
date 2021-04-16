cmake_minimum_required(VERSION 3.12)
project(Physics3D)
set(CMAKE_CXX_STANDARD 20)

include(googletest.cmake)
include(freeglut.cmake)
include(glew.cmake)
include(sdl2.cmake)

add_subdirectory(lib/imgui)

add_subdirectory(common)
add_subdirectory(math)
add_subdirectory(engine)
add_subdirectory(application)
add_subdirectory(preview)

