project(Macro)
set(CMAKE_CXX_LANGUAGE 20)

set(SRC_FILES 
	src/GPU/CommandBuffer.cpp
	src/GPU/GPUDevice.cpp
	src/Math/float2.cpp
	src/Renderer.cpp
	src/Window.cpp)

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
	include/Window.h)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${INC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static spdlog DirectXMath)