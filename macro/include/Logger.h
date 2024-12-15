#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base.h>

template<typename Mutex>
class exception_sink : public spdlog::sinks::base_sink<Mutex>{
public:
  exception_sink() = default;
protected:
  void sink_it(const spdlog::details::log_msg& msg) override
  {
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
  }
};