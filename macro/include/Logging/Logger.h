#pragma once
#include "Sinks.h"
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

class Logger {
public:
	Logger(bool consoleSink = true, bool fileSink = true) {

		if (consoleSink)
		{
			auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			m_sinks.push_back(console_sink);
		}

		if (fileSink)
		{
			auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log.txt", 1024 * 1024, 5, true);
			m_sinks.push_back(file_sink);
		}
#if NDEBUG
		auto sdl_sink = std::make_shared<sdl_messagebox_sink_mt>();
		sdl_sink->set_level(spdlog::level::critical);
		m_sinks.push_back(sdl_sink);
#else
		auto exception_sink = std::make_shared<macro_exception_sink_mt>();
		exception_sink->set_level(spdlog::level::critical);
		m_sinks.push_back(exception_sink);
#endif
		m_logger = std::make_shared<spdlog::logger>("macro_logger", begin(m_sinks), end(m_sinks));
		spdlog::register_logger(m_logger);
		spdlog::set_default_logger(m_logger);
	}
private:
	std::vector<spdlog::sink_ptr> m_sinks;
	std::shared_ptr<spdlog::logger> m_logger;
};
