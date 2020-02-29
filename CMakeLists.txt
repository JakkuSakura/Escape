cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Escape)

set(CMAKE_CXX_STANDARD 17)
#
# Building in-tree is not allowed (we take care of your craziness).
#
 
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake          from there. Thank you.")
endif()

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE SOURCES_MAP_CONVERTER editor/*.cpp)


include_directories(vendor/glm vendor/entt/src vendor/json/single_include vendor/ThorsSerializer)

# specify which version and components you need
find_package(OGRE 1.12 REQUIRED COMPONENTS Bites RTShaderSystem)
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})
add_definitions(${OGRE_DEFINITIONS})
# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(Box2D REQUIRED)
include_directories(${BOX2D_INCLUDE_DIRS})
link_directories(${BOX2D_LIBRARY_DIRS})
add_definitions(${BOX2D_DEFINITIONS})

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

add_executable(main ${SOURCES})
add_executable(map_converter ${SOURCES_MAP_CONVERTER})

########################################################
# Linking & stuff
#########################################################
target_link_libraries(main ${OGRE_LIBRARIES} ${BOX2D_LIBRARIES} ${LUA_LIBRARIES})

add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                ${CMAKE_BINARY_DIR}/assets)
