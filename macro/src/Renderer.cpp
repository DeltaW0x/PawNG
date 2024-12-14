#include "Renderer.h"
#include <spdlog/spdlog.h>

#ifdef NDEBUG
    #define BUFFER_SIZE 128 * 1024 * 1024 
#else
    #define BUFFER_SIZE 128 * 1024 * 1024 
#endif

#define CHECK_TEXTURE(device,texture,info)                                                \
texture = SDL_CreateGPUTexture(device,&info);                                             \
if(!texture)                                                                              \
    throw std::runtime_error(std::format("Failed to create texture: {}",SDL_GetError()));                                                           \

#define CHECK_BUFFER(device,buffer,info)                                                 \
buffer = SDL_CreateGPUBuffer(device,&info);                                              \
if(!buffer)                                                                              \
    throw std::runtime_error(std::format("Failed to create buffer: {}",SDL_GetError()));

Renderer::Renderer(SDL_Window *window,
                   SDL_GPUDevice *device,
                   SDL_GPUTextureFormat colorTargetFormat) : m_window(window), m_device(device)
{
    SDL_GetWindowSizeInPixels(
        m_window,
        reinterpret_cast<int *>(&m_lastWidth),
        reinterpret_cast<int *>(&m_lastHeight));

    m_colorTargetFmt = colorTargetFormat;
    m_depthTargetFmt = QueryDepthStencilFmt();

    m_colorTargetInfo =
    {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = m_colorTargetFmt,
        .usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = m_lastWidth,
        .height = m_lastHeight,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1
    };

    m_depthTargetInfo =
    {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = m_depthTargetFmt,
        .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        .width = m_lastWidth,
        .height = m_lastHeight,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1
    };

    m_finalBlitInfo = {
        .source = {
            .mip_level = 0,
            .layer_or_depth_plane = 0,
            .x = 0,
            .y = 0,
            .w = m_lastWidth,
            .h = m_lastHeight
        },
        .destination = {
            .mip_level = 0,
            .layer_or_depth_plane = 0,
            .x = 0,
            .y = 0,
            .w = m_currWidth,
            .h = m_currHeight
        },
        .load_op = SDL_GPU_LOADOP_DONT_CARE,
        .flip_mode = SDL_FLIP_NONE,
        .filter = SDL_GPU_FILTER_LINEAR
    };

    CHECK_TEXTURE(m_device, m_colorTarget, m_colorTargetInfo);
    CHECK_TEXTURE(m_device, m_depthTarget, m_depthTargetInfo);
    SDL_SetGPUTextureName(m_device,m_colorTarget,"main_color_target");
    SDL_SetGPUTextureName(m_device,m_depthTarget,"main_depth_target");

    SDL_GPUBufferCreateInfo vertexBufferInfo = {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = BUFFER_SIZE
    };

    SDL_GPUBufferCreateInfo indexBufferInfo = {
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = BUFFER_SIZE
    };

    CHECK_BUFFER(m_device,m_vertexBuffer,vertexBufferInfo);
    CHECK_BUFFER(m_device,m_indexBuffer,indexBufferInfo);
    SDL_SetGPUBufferName(m_device,m_vertexBuffer,"main_vertex_buffer");
    SDL_SetGPUBufferName(m_device,m_indexBuffer,"main_index_buffer");
}

SDL_GPUTextureFormat Renderer::QueryDepthStencilFmt() const
{
    auto type = SDL_GPU_TEXTURETYPE_2D;
    auto usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    if (SDL_GPUTextureSupportsFormat(m_device, SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT, type, usage))
    {
        return SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT;
    }
    if (SDL_GPUTextureSupportsFormat(m_device, SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT, type, usage))
    {
        return SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT;
    }
    throw std::runtime_error("Cannot find a valid depth stencil texture format");
}

SDL_GPUCommandBuffer *Renderer::StartRenderingPipeline()
{
    m_mainCmd = SDL_AcquireGPUCommandBuffer(m_device);
    if (!m_mainCmd)
    {
        throw std::runtime_error(std::format("Failed to acquire gpu command buffer: {0}", SDL_GetError()));
    }
    return m_mainCmd;
}

SDL_GPUTexture *Renderer::GetColorTarget() const
{
    return m_colorTarget;
}

SDL_GPUTexture *Renderer::GetDepthTarget() const
{
    return m_depthTarget;
}

void Renderer::PresentFrame()
{
    SDL_GPUTexture *texture;
     if (!SDL_WaitAndAcquireGPUSwapchainTexture(m_mainCmd, m_window, &texture, &m_currWidth, &m_currHeight))
     {
        spdlog::critical("Failed to acquire swapchain texture: {0}", SDL_GetError());
        SDL_CancelGPUCommandBuffer(m_mainCmd);
        return;
     }
    if (texture)
    {
        m_finalBlitInfo.source.texture = m_colorTarget;
        m_finalBlitInfo.source.w = m_lastWidth;
        m_finalBlitInfo.source.h = m_lastHeight;
        m_finalBlitInfo.destination.texture = texture;
        m_finalBlitInfo.destination.w = m_currWidth;
        m_finalBlitInfo.destination.h = m_currHeight;

        SDL_BlitGPUTexture(m_mainCmd, &m_finalBlitInfo);
        SDL_SubmitGPUCommandBuffer(m_mainCmd);
        RecreateRenderTextures();
    }
    else
    {
        SDL_CancelGPUCommandBuffer(m_mainCmd);
    }
}

void Renderer::Quit() const
{
    SDL_WaitForGPUIdle(m_device);
    SDL_ReleaseGPUTexture(m_device, m_colorTarget);
    SDL_ReleaseGPUTexture(m_device, m_depthTarget);
    SDL_ReleaseGPUBuffer(m_device, m_indexBuffer);
    SDL_ReleaseGPUBuffer(m_device, m_vertexBuffer);
}

void Renderer::RecreateRenderTextures()
{
    if (m_lastWidth != m_currWidth || m_lastHeight != m_currHeight)
    {
        SDL_WaitForGPUIdle(m_device);
        SDL_ReleaseGPUTexture(m_device, m_colorTarget);
        SDL_ReleaseGPUTexture(m_device, m_depthTarget);

        m_colorTargetInfo.width = m_currWidth;
        m_colorTargetInfo.height = m_currHeight;
        m_depthTargetInfo.width = m_currWidth;
        m_depthTargetInfo.height = m_currHeight;

        CHECK_TEXTURE(m_device, m_colorTarget, m_colorTargetInfo);
        CHECK_TEXTURE(m_device, m_depthTarget, m_depthTargetInfo);
        SDL_SetGPUTextureName(m_device,m_colorTarget,"main_color_target");
        SDL_SetGPUTextureName(m_device,m_depthTarget,"main_depth_target");

        m_lastWidth = m_currWidth;
        m_lastHeight = m_currHeight;
    }
}
