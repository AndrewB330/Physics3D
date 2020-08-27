message("Installing GLUT...")
set(FreeglutPath "lib/freeglut")
set(GLUT_INCLUDE_DIRS "${FreeglutPath}/include")
set(GLUT_LIBRARIES freeglut.lib)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLUT_LIBRARY_DIRS "${FreeglutPath}/lib/x64")
    file(  COPY "${FreeglutPath}/bin/x64/freeglut.dll"
            DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}"
            )
else()
    set(GLUT_LIBRARY_DIRS "${FreeglutPath}/lib/x32")
    file(  COPY "${FreeglutPath}/bin/x32/freeglut.dll"
            DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}"
            )
endif()

include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_libraries(${GLUT_LIBRARIES})