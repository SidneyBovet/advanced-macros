#include "macro_player/logging.hpp"

#include "spdlog/logger.h"
#include "spdlog/sinks/msvc_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace macro_player
{
    void Logging::setup_logging(spdlog::level::level_enum log_level)
    {
        try
        {
            // flush every 3 seconds
            spdlog::flush_every(std::chrono::seconds(3));

#if defined(WIN32)
            // output to attached windows debugger
            auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
            msvc_sink->set_level(log_level);
            msvc_sink->set_pattern("[%H:%M:%S.%e] [%n] [%l] [thread %t] %v");
#endif

            // output to console
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(log_level);
            console_sink->set_pattern("[%H:%M:%S.%e] [%n] [%l] [thread %t] %v");

            // create a file rotating logger with 5mb size max and 3 rotated files
            size_t max_size = 1024 * 1024 * 5;
            size_t max_files = 3;
            auto file_sink =
                std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/logs.txt", max_size, max_files, true);
            file_sink->set_level(log_level);
            file_sink->set_pattern("[%a %H:%M:%S.%e] [%n] [%l] [%P-%t] %v");

            auto sink_list = spdlog::sinks_init_list({ msvc_sink, console_sink, file_sink });
            auto logger = std::make_shared<spdlog::logger>("global", sink_list);
            logger->set_level(log_level);
            logger->flush_on(spdlog::level::warn);
            spdlog::set_default_logger(logger);
        }
        catch (const std::exception &e)
        {
            // abandon set up, and log to default (console)
            spdlog::error("Couldn't set up logging: {}", e.what());
        }
    }
}