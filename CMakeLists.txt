cmake_minimum_required(VERSION 3.29)
project(PawStomper)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
        message("FOUND CCACHE")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

include_directories(${CMAKE_SOURCE_DIR}/vendor/$<$<BOOL:${WIN32}>:windows>$<$<BOOL:${APPLE}>:macos>$<$<BOOL:${LINUX}>:linux>/include)
link_directories(${CMAKE_SOURCE_DIR}/vendor/$<$<BOOL:${WIN32}>:windows>$<$<BOOL:${APPLE}>:macos>$<$<BOOL:${LINUX}>:linux>/lib)
add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>)

include(CPM.cmake)
include(CMakeOptions.cmake)
include(CMakeDeps.cmake)

add_subdirectory(engine)