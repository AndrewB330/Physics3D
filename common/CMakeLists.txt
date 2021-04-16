# LIBRARY

set(
        HEADERS
        include/common/random.hpp
        include/common/transform.hpp
        include/common/time_utils.hpp
        include/common/assert.hpp
        include/common/lodepng.hpp
        include/common/bit_utils.hpp
        include/common/logging.hpp
)

set(
        SOURCES
        src/random.cpp
        src/time_utils.cpp
        src/lodepng.cpp
        src/logging.cpp
        src/transform.cpp)

add_library(common ${HEADERS} ${SOURCES})

target_include_directories(common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(common PUBLIC math)