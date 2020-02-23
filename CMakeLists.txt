cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Escape)

set(CMAKE_CXX_STANDARD 11)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/) 
file(GLOB SOURCES src/*.cpp)

include_directories(vendor/glm/ vendor/stb/ vendor/ECS/ vendor/asio/asio/include)

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
add_executable(main ${SOURCES})

########################################################
# Linking & stuff
#########################################################
target_link_libraries(main ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})
# target_link_libraries(client ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} )
# target_link_libraries(server)

add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                ${CMAKE_BINARY_DIR}/assets)
