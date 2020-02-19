cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(HW_OPENGL)
if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()
include_directories(vendor/glm/)
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
# FIND GLFW
#########################################################
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
add_definitions(${GLFW_DEFINITIONS})
if(NOT GLFW_FOUND)
    message("Error: GLFW not found!")
endif(NOT GLFW_FOUND)
#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message("Error: GLEW not found!")
endif(NOT GLEW_FOUND)
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
target_link_libraries(main ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})
# target_link_libraries(client ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} )
# target_link_libraries(server)