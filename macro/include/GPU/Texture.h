#include <SDL3/SDL_gpu.h>

class Texture {
public:
    Texture() : m_handle(nullptr), m_width(0), m_height(0)
    {
    }

    unsigned int GetWidth() const { return m_width; }
    unsigned int GetHeight() const { return m_height; }
    operator bool() const
    {
        return m_handle != nullptr;
    }

    explicit operator SDL_GPUTexture*() const
    {
        return m_handle;
    }
private:
    Texture(SDL_GPUTexture* handle) : m_handle(handle), m_width(0), m_height(0)
    {
    }

private:
    friend class GPUDevice;
    friend class CommandBuffer;

    SDL_GPUTexture * m_handle;
    uint32_t m_width;
    uint32_t m_height;
};