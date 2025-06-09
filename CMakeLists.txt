cmake_minimum_required(VERSION 3.16...3.27)

project(OpenSR
    VERSION 0.1.1
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(API_VERSION 1)

find_package(Qt6 REQUIRED COMPONENTS Core Gui)

option(BUILD_ALL_TOOLS "Build all tools (overrides individual tool options)" OFF)
option(BUILD_RESOURCE_VIEWER "Build Resource Viewer tool" OFF)
option(BUILD_PLANET_VIEWER "Build Planet Viewer tool" OFF)
option(BUILD_QUEST_PLAYER "Build Quest Player tool" OFF)
option(BUILD_DAT_TOOLS "Build tools for DAT files" OFF)

if(BUILD_ALL_TOOLS)
    set(BUILD_RESOURCE_VIEWER ON)
    set(BUILD_PLANET_VIEWER ON)
    set(BUILD_QUEST_PLAYER ON)
    set(BUILD_DAT_TOOLS ON)
endif()

add_subdirectory(Ranger)
add_subdirectory(ImagePlugin)
add_subdirectory(QM)
add_subdirectory(Engine)
add_subdirectory(World)

if(BUILD_RESOURCE_VIEWER OR BUILD_PLANET_VIEWER OR BUILD_QUEST_PLAYER OR BUILD_DAT_TOOLS)
    add_subdirectory(tools)
endif()
