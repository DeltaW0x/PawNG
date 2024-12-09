cmake_minimum_required(VERSION 3.29)
project(MacroPaw)
include(CPM.cmake)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(${CMAKE_SOURCE_DIR}/vendor/$<$<BOOL:${WIN32}>:windows>$<$<BOOL:${APPLE}>:macos>$<$<BOOL:${LINUX}>:linux>/include)
link_directories(${CMAKE_SOURCE_DIR}/vendor/$<$<BOOL:${WIN32}>:windows>$<$<BOOL:${APPLE}>:macos>$<$<BOOL:${LINUX}>:linux>/lib)

CPMAddPackage(
    NAME SDL3
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG main
    OPTIONS "SDL_TESTS OFF" "SDL_TEST_LIBRARY OFF" "SDL_STATIC ON" "SDL_SHARED OFF")

CPMAddPackage(
    NAME SDL_shadercross
    GITHUB_REPOSITORY libsdl-org/SDL_shadercross
    GIT_TAG main
    GIT_SUBMODULES_RECURSE ON
    OPTIONS "SDLSHADERCROSS_VENDORED ON" "SDLSHADERCROSS_CLI OFF" "SDLSHADERCROSS_STATIC ON" "SDLSHADERCROSS_SHARED OFF" "SDLSHADERCROSS_SPIRVCROSS_SHARED OFF")

CPMAddPackage(
    NAME json
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG v3.11.3)

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.15.0)

add_subdirectory(macro)
add_subdirectory(stomper)