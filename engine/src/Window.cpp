#include "Window.h"
#include <spdlog/spdlog.h>

Window::Window(const std::string &title, unsigned int width,
               unsigned int height, int flags)
    : m_fullscreen(false), m_borderless(true) {
  m_handle = SDL_CreateWindow(title.c_str(), static_cast<int>(width),
                              static_cast<int>(height), flags);
  if (!m_handle) {
    spdlog::critical("Failed to create window: {0}", SDL_GetError());
  }
}

Window::~Window() { SDL_DestroyWindow(m_handle); }

bool Window::SetFullscreen(bool fullscreen, bool borderless) {
  if (fullscreen != m_fullscreen) {
    if (!SDL_SetWindowFullscreen(m_handle, fullscreen)) {
      spdlog::error("Failed to set window fullscreen: {0}", SDL_GetError());
      return false;
    }
    if (m_borderless != borderless) {
      if (!borderless) {
        if (!SDL_SetWindowFullscreenMode(
                m_handle, SDL_GetFullscreenDisplayModes(
                              SDL_GetDisplayForWindow(m_handle), nullptr)[0])) {
          spdlog::error("Failed to query SDL fullscreen modes: {0}",
                        SDL_GetError());
          return false;
        }
      }
    }
  }
  m_fullscreen = fullscreen;
  m_borderless = borderless;
  return true;
}

bool Window::IsFullscreen() { return m_fullscreen; }

bool Window::IsBorderless() { return m_borderless; }

bool Window::SetSize(const int2 &size) {
  if (!SDL_SetWindowSize(m_handle, size.x, size.y)) {
    spdlog::error("Failed to set window size: {0}", SDL_GetError());
    return false;
  }
  return true;
}

bool Window::SetPosition(const int2 &position) {
  if (!SDL_SetWindowPosition(m_handle, position.x, position.y)) {
    spdlog::error("Failed to set window position: {0}", SDL_GetError());
    return false;
  }
  return true;
}

bool Window::SetTitle(std::string_view title) {
  if (!SDL_SetWindowTitle(m_handle, title.data())) {
    spdlog::error("Failed to set window size: {0}", SDL_GetError());
    return false;
  }
  return true;
}

std::string Window::GetTitle() const {
  return std::string(SDL_GetWindowTitle(m_handle));
}

int2 Window::GetSize() const {
  int width, height;
  SDL_GetWindowSizeInPixels(m_handle, &width, &height);
  return int2(width, height);
}

int2 Window::GetPosition() const {
  int x, y;
  SDL_GetWindowPosition(m_handle, &x, &y);
  return int2(x, y);
}

SDL_DisplayID Window::GetDisplay() const {
  return SDL_GetDisplayForWindow(m_handle);
}

Window::operator SDL_Window *() const { return m_handle; }
