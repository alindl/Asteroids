set( openGL_demo_SRC
#  add additional files as needed
	)

find_package(OpenGL REQUIRED)
if( OPENGL_FOUND )
    set(openGL_demo_LIBS ${openGL_demo_LIBS} ${OPENGL_LIBRARY})
else()               
    message(ERROR "OPENGL not found!")
endif()


if( WITH_GLUT )
    find_package(GLUT REQUIRED)
    if( GLUT_FOUND )
        message("Adding OpenGL GLUT support")
        set(openGL_demo_LIBS ${openGL_demo_LIBS} ${GLUT_LIBRARY})
    else()               
        message(ERROR "GLUT not found!")
    endif()
endif()      

if( WITH_GLFW )
    find_package(GLFW REQUIRED)
    if( GLFW_FOUND )
        message("Adding OpenGL GLFW support")
        set(openGL_demo_LIBS ${openGL_demo_LIBS} ${GLFW_LIBRARY})
        include_directories(${GLFW_INCLUDE_DIR})
    else()               
        message(ERROR "GLFW not found!")
    endif()
endif()

if( WITH_GLEW )
    find_package(GLEW REQUIRED)
    if( GLEW_FOUND )
        message("Adding OpenGL GLEW support")
        set(openGL_demo_LIBS ${openGL_demo_LIBS} ${GLEW_LIBRARY})
        include_directories(${GLEW_INCLUDE_DIR})
    else()               
        message(ERROR "GLEW not found!")
    endif()
endif()

if( WITH_SOIL )
    find_package(SOIL REQUIRED)
    if( SOIL_FOUND )
        message("Adding OpenGL SOIL support")
        set(openGL_demo_LIBS ${openGL_demo_LIBS} ${SOIL_LIBRARY})
        include_directories(${SOIL_INCLUDE_DIR})
    else()               
        message(ERROR "SOIL not found!")
    endif()
endif()


# name der datei mit main() des projektes
# executable heisst "openGL_demo"
#add_executable(openGL_demo ${openGL_demo_SRC} window.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} drawing.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} colored_tri.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} colored_quad.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} transformed_quad.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} spinning_quad.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} mvp_quad.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} callbacks_quad.cc) 
#add_executable(openGL_demo ${openGL_demo_SRC} textured_quad.cc) 
add_executable(openGL_demo ${openGL_demo_SRC} textured_cube.cc) 

target_link_libraries(openGL_demo ${openGL_demo_LIBS}
-lXi 
-lXxf86vm 
-lXinerama 
-lXcursor 
-lXrandr 
-lX11 
-lpthread 
-ldl
)
	
set_target_properties(openGL_demo PROPERTIES COMPILE_FLAGS "")
