#include <SDL3/SDL_gpu.h>

class Fence {
public:
  Fence() : m_handle(nullptr) {}

  explicit operator SDL_GPUFence *() const { return m_handle; }

private:
  Fence(SDL_GPUFence *handle) : m_handle(handle) {}

private:
  friend class GPUDevice;
  friend class CommandBuffer;

  SDL_GPUFence *m_handle;
};
