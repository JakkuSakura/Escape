cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Escape)

set(CMAKE_CXX_STANDARD 17)
#
# Building in-tree is not allowed (we take care of your craziness).
#
 
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

file(GLOB_RECURSE SOURCES_CORE core/*.cpp)

include_directories(core vendor/glm vendor/entt/src vendor/json/single_include vendor/ThorsSerializer)

find_package(Box2D REQUIRED)
include_directories(${BOX2D_INCLUDE_DIRS})
link_directories(${BOX2D_LIBRARY_DIRS})
add_definitions(${BOX2D_DEFINITIONS})

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

add_subdirectory(client/ogre)
add_subdirectory(client/cocos2dx)
