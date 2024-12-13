project(Stomper)
set(CMAKE_CXX_LANGUAGE 20)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE Macro nlohmann_json::nlohmann_json SDL3_shadercross-static)

add_custom_target(copy_sharedlibs 
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-sharedlibs.cmake)
add_dependencies(${PROJECT_NAME} copy_sharedlibs)