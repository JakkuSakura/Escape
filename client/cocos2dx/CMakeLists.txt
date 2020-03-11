cmake_minimum_required(VERSION 3.6)

set(APP_NAME main_cocos2dx)

if (XCODE)
    if (NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif ()
endif ()

if (NOT DEFINED BUILD_ENGINE_DONE) # to test Escape into root project
    set(COCOS2DX_ROOT_PATH /home/jack/IdeaProjects/cocos2d-x-4.0)
    # TODO Must setup cocos2dx path before building
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)
    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif ()
set(CMAKE_CXX_STANDARD 17)
# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        )
if (APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif ()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
        Classes/AppDelegate.cpp
        Classes/GameScene.cpp
        Classes/Settings.cpp
        )
list(APPEND GAME_HEADER
        Classes/AppDelegate.h
        Classes/GameScene.h
        Classes/Settings.h
        )
if (LINUX)
    list(APPEND GAME_SOURCE
            proj.linux/main.cpp
            )
elseif (WINDOWS)
    list(APPEND GAME_HEADER
            proj.win32/main.h
            proj.win32/resource.h
            )
    list(APPEND GAME_SOURCE
            proj.win32/main.cpp
            proj.win32/game.rc
            ${common_res_files}
            )

endif ()

# mark app complie info and libs info
set(all_code_files
        ${GAME_HEADER}
        ${GAME_SOURCE}
        ${SOURCES_CORE}
        )
if (NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
endif ()

target_link_libraries(${APP_NAME} cocos2d ${BOX2D_LIBRARIES} ${LUA_LIBRARIES})
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
        )

# mark app resources
setup_cocos_app_config(${APP_NAME})

if (WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif ()

if (LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif ()
