#include "Window.h"
#include <stdexcept>
#include <__format/format_functions.h>

Window::Window(const std::string& title, unsigned int width, unsigned int height,int flags)
{
    m_handle = SDL_CreateWindow(title.c_str(),static_cast<int>(width),static_cast<int>(height),flags);
    if(!m_handle)
    {
        throw std::runtime_error(std::format("Failed to create window: {}",SDL_GetError()));
    }
}

Window::~Window()
{
    SDL_DestroyWindow(m_handle);
}

std::string Window::GetTitle() const
{
    return std::string(SDL_GetWindowTitle(m_handle));
}

float2 Window::GetSize() const
{
    int width, height;
    SDL_GetWindowSizeInPixels(m_handle,&width,&height);
    return float2(width,height);
}

float2 Window::GetPosition() const
{
    int x, y;
    SDL_GetWindowPosition(m_handle,&x,&y);
    return float2(x,y);
}

Window::operator SDL_Window*() const
{
    return m_handle;
}

