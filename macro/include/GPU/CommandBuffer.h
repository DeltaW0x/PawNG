#pragma once
#include "Window.h"
#include "Texture.h"
#include "RenderPass.h"
#include <vector>
class CommandBuffer {
public:
    CommandBuffer();

    bool AcquireSwapchainTexture(const Window &window, Texture& texture) const;
    bool WaitAndAcquireSwapchainTexture(const Window &window, Texture& texture) const;

    RenderPass BeginRenderPass(const SDL_GPUColorTargetInfo &colorTargetInfo) const;
    RenderPass BeginRenderPass(const SDL_GPUDepthStencilTargetInfo& depthStencilTargetInfo) const;
    RenderPass BeginRenderPass(const SDL_GPUColorTargetInfo& colorTargetInfo, const SDL_GPUDepthStencilTargetInfo& depthStencilTargetInfo) const;
    RenderPass BeginRenderPass(const std::vector<SDL_GPUColorTargetInfo>& colorTargetInfo, const SDL_GPUDepthStencilTargetInfo& depthStencilTargetInfo) const;
    void EndRenderPass(RenderPass& renderPass) const;

    void BlitTexture(const SDL_GPUBlitInfo& info) const;

    explicit operator SDL_GPUCommandBuffer*() const;

private:
    CommandBuffer(SDL_GPUCommandBuffer* handle);

private:
    friend class GPUDevice;
    SDL_GPUCommandBuffer* m_handle;
};

