#pragma once
#include "Math/int2.h"
#include <SDL3/SDL_video.h>
#include <string>

class Window {
public:
  Window(const std::string &title, unsigned int width, unsigned int height,
         int flags = SDL_WINDOW_HIGH_PIXEL_DENSITY);
  ~Window();

  bool SetFullscreen(bool fullscreen, bool borderless = false);
  bool IsFullscreen();
  bool IsBorderless();

  bool SetSize(const int2 &size);
  bool SetPosition(const int2 &position);
  bool SetTitle(std::string_view title);

  int2 GetSize() const;
  int2 GetPosition() const;
  std::string GetTitle() const;
  SDL_DisplayID GetDisplay() const;

  explicit operator SDL_Window *() const;

private:
  friend class GPUDevice;
  friend class CommandBuffer;
  SDL_Window *m_handle;
  bool m_fullscreen;
  bool m_borderless;
};