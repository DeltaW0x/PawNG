project(Macro)
set(CMAKE_CXX_LANGUAGE 20)

set(SRC_FILES
        src/Window.cpp
        src/GPU/GPUDevice.cpp
        src/GPU/CommandBuffer.cpp
        src/Math/float2.cpp
        include/Window.h
        include/Renderer.h
        include/Logger.h
        include/GPU/GPUDevice.h
        include/GPU/CommandBuffer.h
        include/GPU/Texture.h
        include/GPU/Fence.h
        include/GPU/RenderPass.h
        include/Math/float2.h
        include/sal.h)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static spdlog DirectXMath)