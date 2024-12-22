#include "GPU/CommandBuffer.h"

#include <vector>

CommandBuffer::CommandBuffer() : m_handle(nullptr) {}

CommandBuffer::CommandBuffer(SDL_GPUCommandBuffer *handle) : m_handle(handle) {}

bool CommandBuffer::AcquireSwapchainTexture(const Window &window,
                                            Texture &texture) const {
  return SDL_AcquireGPUSwapchainTexture(m_handle, window.m_handle,
                                        &texture.m_handle, &texture.m_width,
                                        &texture.m_height);
}

bool CommandBuffer::WaitAndAcquireSwapchainTexture(const Window &window,
                                                   Texture &texture) const {
  return SDL_WaitAndAcquireGPUSwapchainTexture(
      m_handle, window.m_handle, &texture.m_handle, &texture.m_width,
      &texture.m_height);
}

RenderPass CommandBuffer::BeginRenderPass(
    const SDL_GPUColorTargetInfo &colorTargetInfo) const {
  return RenderPass(
      SDL_BeginGPURenderPass(m_handle, &colorTargetInfo, 1, nullptr));
}

RenderPass CommandBuffer::BeginRenderPass(
    const SDL_GPUDepthStencilTargetInfo &depthStencilTargetInfo) const {
  return RenderPass(
      SDL_BeginGPURenderPass(m_handle, nullptr, 1, &depthStencilTargetInfo));
}

RenderPass CommandBuffer::BeginRenderPass(
    const SDL_GPUColorTargetInfo &colorTargetInfo,
    const SDL_GPUDepthStencilTargetInfo &depthStencilTargetInfo) const {
  return RenderPass(SDL_BeginGPURenderPass(m_handle, &colorTargetInfo, 1,
                                           &depthStencilTargetInfo));
}

RenderPass CommandBuffer::BeginRenderPass(
    const std::vector<SDL_GPUColorTargetInfo> &colorTargetInfo,
    const SDL_GPUDepthStencilTargetInfo &depthStencilTargetInfo) const {
  return RenderPass(SDL_BeginGPURenderPass(m_handle, colorTargetInfo.data(),
                                           colorTargetInfo.size(),
                                           &depthStencilTargetInfo));
}

void CommandBuffer::EndRenderPass(RenderPass &renderPass) const {
  SDL_EndGPURenderPass(renderPass.m_handle);
  renderPass.m_handle = nullptr;
}

void CommandBuffer::BlitTexture(const SDL_GPUBlitInfo &info) const {
  SDL_BlitGPUTexture(m_handle, &info);
}

CommandBuffer::operator struct SDL_GPUCommandBuffer *() const {
  return m_handle;
}
