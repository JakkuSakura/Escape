cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE SOURCES_CLIENT_OGRE src/*.cpp)

# specify which version and components you need
find_package(OGRE 1.12 REQUIRED COMPONENTS Bites RTShaderSystem)
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})
add_definitions(${OGRE_DEFINITIONS})
# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(main_ogre ${SOURCES_CORE} ${SOURCES_CLIENT_OGRE})
target_link_libraries(main_ogre ${OGRE_LIBRARIES} ${BOX2D_LIBRARIES} ${LUA_LIBRARIES})