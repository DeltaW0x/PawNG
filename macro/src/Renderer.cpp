#include "Renderer.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

Renderer::Renderer(
    Window &window,
    GPUDevice &device,
    SDL_GPUTextureFormat colorTargetFormat) : m_window(window), m_device(device)
{
    float2 windowSize = m_window.GetSize();
    m_lastWidth = windowSize.x;
    m_lastHeight = windowSize.y;

    m_colorTargetFmt = colorTargetFormat;
    m_depthTargetFmt = QueryDepthStencilFmt();

    m_colorTargetInfo = {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = m_colorTargetFmt,
        .usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = m_lastWidth,
        .height = m_lastHeight,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1
    };

    m_depthTargetInfo = {
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
        .filter = SDL_GPU_FILTER_NEAREST
    };

    m_colorTarget = m_device.CreateTexture(m_colorTargetInfo);
    m_depthTarget = m_device.CreateTexture(m_depthTargetInfo);
}

SDL_GPUTextureFormat Renderer::QueryDepthStencilFmt() {
    auto type = SDL_GPU_TEXTURETYPE_2D;
    auto usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    if (m_device.SupportsTextureFormat(SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT, type, usage)) {
        return SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT;
    }
    if (m_device.SupportsTextureFormat(SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT, type, usage)) {
        return SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT;
    }
    throw std::runtime_error("Cannot find a valid depth stencil texture format");
}

CommandBuffer& Renderer::StartRenderingPipeline()
{
    m_mainCmd = m_device.AcquireCommandBuffer();
    return m_mainCmd;
}

Texture& Renderer::GetColorTarget() {
    return m_colorTarget;
}

Texture& Renderer::GetDepthTarget() {
    return m_depthTarget;
}

void Renderer::PresentFrame() {
    Texture texture;
    if (!m_mainCmd.WaitAndAcquireSwapchainTexture(m_window,texture)) {
        spdlog::critical("Failed to acquire swapchain texture: {0}", SDL_GetError());
        m_device.CancelCommandBuffer(m_mainCmd);
        return;
    }
    if (texture) {
        m_currWidth = texture.GetWidth();
        m_currHeight = texture.GetHeight();
        m_finalBlitInfo.source.texture = static_cast<SDL_GPUTexture *>(m_colorTarget);
        m_finalBlitInfo.source.w = m_lastWidth;
        m_finalBlitInfo.source.h = m_lastHeight;
        m_finalBlitInfo.destination.texture = static_cast<SDL_GPUTexture *>(texture);
        m_finalBlitInfo.destination.w = m_currWidth;
        m_finalBlitInfo.destination.h = m_currHeight;

        m_mainCmd.BlitTexture(m_finalBlitInfo);
        m_device.SubmitCommandBuffer(m_mainCmd);
        RecreateRenderTextures();
    } else {
        m_device.CancelCommandBuffer(m_mainCmd);
    }
}

void Renderer::Quit() {
    m_device.WaitForIdle();
    m_device.ReleaseTexture(m_colorTarget);
    m_device.ReleaseTexture(m_depthTarget);
}

void Renderer::RecreateRenderTextures() {
    if (m_lastWidth != m_currWidth || m_lastHeight != m_currHeight) {
        m_device.WaitForIdle();
        m_device.ReleaseTexture(m_colorTarget);
        m_device.ReleaseTexture(m_depthTarget);

        m_colorTargetInfo.width = m_currWidth;
        m_colorTargetInfo.height = m_currHeight;
        m_depthTargetInfo.width = m_currWidth;
        m_depthTargetInfo.height = m_currHeight;

        m_colorTarget = m_device.CreateTexture(m_colorTargetInfo);
        m_depthTarget = m_device.CreateTexture(m_depthTargetInfo);
        m_lastWidth = m_currWidth;
        m_lastHeight = m_currHeight;
    }
}
