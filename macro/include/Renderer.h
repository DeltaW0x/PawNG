#pragma once
#include <SDL3/SDL.h>

class Renderer {
public:
    Renderer(SDL_Window *window,
             SDL_GPUDevice *device,
             SDL_GPUTextureFormat colorTargetFormat = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT);

    SDL_GPUCommandBuffer *StartRenderingPipeline();
    SDL_GPUTexture *GetColorTarget() const;
    SDL_GPUTexture *GetDepthTarget() const;

    void PresentFrame();
    void Quit() const;
private:
    SDL_GPUTextureFormat QueryDepthStencilFmt() const;
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
    SDL_GPUBuffer* m_vertexBuffer;
    SDL_GPUBuffer* m_indexBuffer;

    SDL_GPUBlitInfo m_finalBlitInfo;
    uint32_t m_lastWidth, m_lastHeight;
    uint32_t m_currWidth, m_currHeight;
};
