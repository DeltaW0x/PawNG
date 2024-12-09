project(Macro)
set(CMAKE_CXX_LANGUAGE 20)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static spdlog)