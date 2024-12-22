#pragma once
#include <SDL3/SDL.h>
#include <mutex>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <stdexcept>

template <typename Mutex>
class sdl_messagebox_sink : public spdlog::sinks::base_sink<Mutex> {
public:
  sdl_messagebox_sink() {}

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg.payload.data(),
                             nullptr);
    SDL_Event e;
    e.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&e);
  }

  void flush_() override {}
};

using sdl_messagebox_sink_mt = sdl_messagebox_sink<std::mutex>;
using sdl_messagebox_sink_st = sdl_messagebox_sink<spdlog::details::null_mutex>;

template <typename Mutex>
class macro_exception_sink : public spdlog::sinks::base_sink<Mutex> {
public:
  macro_exception_sink() {}

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override { abort(); }

  void flush_() override {}
};

using macro_exception_sink_mt = macro_exception_sink<std::mutex>;
using macro_exception_sink_st =
    macro_exception_sink<spdlog::details::null_mutex>;