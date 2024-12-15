#pragma once
#include "Fence.h"
#include "Window.h"
#include "CommandBuffer.h"

class GPUDevice 
{
public:
    explicit GPUDevice(SDL_GPUShaderFormat shaderFormats, bool debug = false, const char* driver = nullptr);
    ~GPUDevice();

    bool WaitForIdle() const;
    bool WaitForSwapchain(const Window& window) const;

    bool ClaimWindow(const Window& window) const;
    void ReleaseWindow(const Window& window) const;

    CommandBuffer AcquireCommandBuffer() const;
    bool          CancelCommandBuffer(CommandBuffer& cmd) const;
    void          SubmitCommandBuffer(CommandBuffer& cmd) const;
    Fence         SubmitCommandBufferWithFence(CommandBuffer& cmd) const;

    Texture CreateTexture(const SDL_GPUTextureCreateInfo& info);
    void    ReleaseTexture(Texture& texture);
    bool    SupportsTextureFormat(SDL_GPUTextureFormat format, SDL_GPUTextureType type, SDL_GPUTextureUsageFlags usage);

    explicit operator SDL_GPUDevice*() const;
private:
    SDL_GPUDevice* m_device;
};
