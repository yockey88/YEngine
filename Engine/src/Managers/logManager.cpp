#include "Managers/logManager.hpp"

#include <iostream>
#include <ctime>
#include <chrono>

namespace machy {
namespace managers {

    void LogManager::init() {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto errSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();

        consoleSink->set_pattern("[$$$] >>> [ %v ] <<< [%D %H:%M:%S.%e %Y] [%n <{thread}-> %t , {process}-> %P>]");
        consoleSink->set_level(spdlog::level::trace);

        errSink->set_pattern("{!!! | <%l>} >>> { %v } <<< {%D %H:%M:%S.%e %Y} [%n <{thread}-> %t , {process}-> %P>]");
        errSink->set_level(spdlog::level::warn);

        std::vector<spdlog::sink_ptr> sinks1 { consoleSink };
        MainLogger = std::make_shared<spdlog::logger>(MACHY_DEFAULT_LOGGER_NAME , sinks1.begin() , sinks1.end());
        MainLogger->set_level(spdlog::level::trace);
        MainLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(MainLogger);

        std::vector<spdlog::sink_ptr> sinks2 { errSink };
        ErrLogger = std::make_shared<spdlog::logger>(MACHY_ERROR_LOGGER_NAME , sinks2.begin() , sinks2.end());
        ErrLogger->set_level(spdlog::level::warn);
        ErrLogger->flush_on(spdlog::level::warn);
        spdlog::register_logger(ErrLogger);

        open = true;

        return;
    }

    void LogManager::shutdown() {
        MACHY_TRACE("Closing Log");

        spdlog::shutdown();

        return;
    }

}
}