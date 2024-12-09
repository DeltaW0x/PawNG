#pragma once
#include <SDL3/SDL.h>

class Renderer {
public:
    Renderer(SDL_Window *window,
             SDL_GPUDevice *device,
             SDL_GPUTextureFormat colorTargetFormat = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT);

    SDL_GPUCommandBuffer *StartRenderingPipeline();
    SDL_GPUTexture *GetColorTarget();
    SDL_GPUTexture *GetDepthTarget();

    void PresentFrame();
    void Quit();
private:
    SDL_GPUTextureFormat QueryDepthStencilFmt();
    void RecreateRenderTextures();

    SDL_Window *m_window;
    SDL_GPUDevice *m_device;
    SDL_GPUTextureFormat m_depthTargetFmt;
    SDL_GPUTextureFormat m_colorTargetFmt;
    SDL_GPUTextureCreateInfo m_depthTargetInfo;
    SDL_GPUTextureCreateInfo m_colorTargetInfo;
    SDL_GPUTexture *m_colorTarget;
    SDL_GPUTexture *m_depthTarget;

    SDL_GPUCommandBuffer *m_mainCmd;
    SDL_GPUBuffer* m_staticVertexBuffer;
    SDL_GPUBuffer* m_staticIndexBuffer;

    SDL_GPUBlitInfo m_finalBlitInfo;
    uint32_t m_lastWidth, m_lastHeight;
    uint32_t m_currWidth, m_currHeight;
};
