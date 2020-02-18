cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(HW_OPENGL)
if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()
#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message("Error: GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message("Error: OPENGL not found!")
endif(NOT OPENGL_FOUND)
#########################################################
# Include Files
#########################################################
# add_executable(server server.cpp)
# add_executable(client client.cpp)
add_executable(main src/client.cpp src/server.cpp)

########################################################
# Linking & stuff
#########################################################
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
# target_link_libraries(client ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
# target_link_libraries(server)