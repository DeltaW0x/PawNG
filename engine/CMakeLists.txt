project(Engine)
set(CMAKE_CXX_LANGUAGE 17)

set(SRC_FILES
        src/GPU/CommandBuffer.cpp
        src/GPU/GPUDevice.cpp
        src/Math/float2.cpp
        src/Renderer.cpp
        src/Window.cpp
        src/Math/int2.cpp
        src/Lua/LuaContext.cpp)

set(INC_FILES
        include/GPU/CommandBuffer.h
        include/GPU/GPUDevice.h
        include/GPU/Fence.h
        include/GPU/RenderPass.h
        include/GPU/Texture.h
        include/Logging/Logger.h
        include/Logging/Sinks.h
        include/Math/float2.h
        include/Renderer.h
        include/sal.h
        include/Window.h
        include/Math/int2.h
        include/Lua/LuaContext.h)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${INC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static spdlog DirectXMath nlohmann_json::nlohmann_json LuaBridge juajit-5.1)