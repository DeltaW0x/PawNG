#pragma once
#include "GPU/GPUDevice.h"
#include "Window.h"
class Renderer {
public:
    Renderer(
            Window& window,
            GPUDevice& device,
            SDL_GPUTextureFormat colorTargetFormat = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT);

    CommandBuffer& StartRenderingPipeline();
    Texture&       GetColorTarget();
    Texture&       GetDepthTarget();

    void PresentFrame();
    void Quit();
private:
    SDL_GPUTextureFormat QueryDepthStencilFmt();
    void RecreateRenderTextures();

    Window& m_window;
    GPUDevice& m_device;
    SDL_GPUTextureFormat m_depthTargetFmt;
    SDL_GPUTextureFormat m_colorTargetFmt;
    SDL_GPUTextureCreateInfo m_depthTargetInfo;
    SDL_GPUTextureCreateInfo m_colorTargetInfo;
    Texture m_colorTarget;
    Texture m_depthTarget;

    CommandBuffer m_mainCmd;

    SDL_GPUBlitInfo m_finalBlitInfo;
    uint32_t m_lastWidth, m_lastHeight;
    uint32_t m_currWidth, m_currHeight;
};
