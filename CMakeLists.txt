cmake_minimum_required(VERSION 3.12)
project(Physics3D)
set(CMAKE_CXX_STANDARD 17)

include(googletest.cmake)
include(freeglut.cmake)

add_subdirectory(math)
add_subdirectory(engine)
add_subdirectory(viewer)

add_executable(runnable runnable.cpp)
target_link_libraries(runnable PUBLIC viewer)