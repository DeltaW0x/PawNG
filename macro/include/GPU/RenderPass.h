#pragma once
#include <SDL3/SDL_gpu.h>

class RenderPass {
public:
    RenderPass() : m_handle(nullptr) {}

    explicit operator SDL_GPURenderPass*() const
    {
        return m_handle;
    }

private:
    RenderPass(SDL_GPURenderPass* handle) : m_handle(handle) {}
private:
    friend class GPUDevice;
    friend class CommandBuffer;
    SDL_GPURenderPass* m_handle;
};
