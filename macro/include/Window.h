#pragma once
#include "Math/float2.h"
#include <string>
#include <SDL3/SDL_video.h>


class Window
{
public:
    Window(const std::string& title, unsigned int width, unsigned int height, int flags = SDL_WINDOW_HIGH_PIXEL_DENSITY);
    ~Window();

    std::string GetTitle() const;
    float2 GetSize() const;
    float2 GetPosition() const;

    explicit operator SDL_Window*() const;
private:
    friend class GPUDevice;
    friend class CommandBuffer;
    SDL_Window* m_handle;
};