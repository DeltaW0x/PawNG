#include "GPU/GPUDevice.h"
#include <format>
#include <stdexcept>
GPUDevice::GPUDevice(SDL_GPUShaderFormat shaderFormats, bool debug, const char *driver)
{
    m_device = SDL_CreateGPUDevice(shaderFormats,debug,driver);
    if (!m_device)
    {
        throw std::runtime_error(std::format("Failed to create GPU device: {}", SDL_GetError()));
    }
}

GPUDevice::~GPUDevice()
{
    SDL_WaitForGPUIdle(m_device);
    SDL_DestroyGPUDevice(m_device);
}

bool GPUDevice::WaitForIdle() const
{
    return SDL_WaitForGPUIdle(m_device);
}

bool GPUDevice::WaitForSwapchain(const Window &window) const
{
    return SDL_WaitForGPUSwapchain(m_device,window.m_handle);
}

bool GPUDevice::ClaimWindow(const Window &window) const
{
    return SDL_ClaimWindowForGPUDevice(m_device,window.m_handle);
}

void GPUDevice::ReleaseWindow(const Window &window) const
{
    return SDL_ReleaseWindowFromGPUDevice(m_device,window.m_handle);
}

CommandBuffer GPUDevice::AcquireCommandBuffer() const
{
    CommandBuffer cmd(SDL_AcquireGPUCommandBuffer(m_device));
    if(!cmd.m_handle)
    {
        throw std::runtime_error(std::format("Failed to acquire GPU CommandBuffer: {}", SDL_GetError()));
    }
    return cmd;
}

void GPUDevice::SubmitCommandBuffer(CommandBuffer& cmd) const
{
    if (!SDL_SubmitGPUCommandBuffer(cmd.m_handle))
    {
        cmd.m_handle = nullptr;
        throw std::runtime_error(std::format("Failed to submit GPU CommandBuffer: {}", SDL_GetError()));
    }
    cmd.m_handle = nullptr;
}

Fence GPUDevice::SubmitCommandBufferWithFence(CommandBuffer &cmd) const
{
    Fence fence(SDL_SubmitGPUCommandBufferAndAcquireFence(cmd.m_handle));
    if (!fence.m_handle)
    {
        throw std::runtime_error(std::format("Failed to acquire GPU fence: {}", SDL_GetError()));
    }
    return fence;
}

Texture GPUDevice::CreateTexture(const SDL_GPUTextureCreateInfo &info)
{
    return Texture(SDL_CreateGPUTexture(m_device,&info));
}

void GPUDevice::ReleaseTexture(Texture &texture)
{
    SDL_ReleaseGPUTexture(m_device,texture.m_handle);
}

bool GPUDevice::SupportsTextureFormat(SDL_GPUTextureFormat format, SDL_GPUTextureType type, SDL_GPUTextureUsageFlags usage)
{
    return SDL_GPUTextureSupportsFormat(m_device, format, type, usage);
}

GPUDevice::operator struct SDL_GPUDevice*() const
{
    return m_device;
}

bool GPUDevice::CancelCommandBuffer(CommandBuffer &cmd) const
{
    bool r = SDL_CancelGPUCommandBuffer(cmd.m_handle);
    cmd.m_handle = nullptr;
    return r;
}
