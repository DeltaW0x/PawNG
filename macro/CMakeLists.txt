project(Macro)
set(CMAKE_CXX_LANGUAGE 20)

set(SRC_FILES
        src/Window.cpp
        src/Renderer.cpp
        src/GPU/GPUDevice.cpp
        include/Window.h
        include/Renderer.h
        include/GPU/GPUDevice.h
        src/GPU/CommandBuffer.cpp
        include/GPU/CommandBuffer.h
        include/GPU/Texture.h
        include/GPU/Fence.h
        include/GPU/RenderPass.h
        include/Math/float2.h
        src/Math/float2.cpp
        include/sal.h)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static spdlog DirectXMath)