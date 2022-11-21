#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include "log.hpp"

#include <queue>
#include <string>
#include <fstream>
#include <memory>

namespace machy {

namespace managers {

    class LogManager {
        std::shared_ptr<spdlog::logger> MainLogger;
        std::shared_ptr<spdlog::logger> ErrLogger;
        bool open;

        // Delete Copy Constructors
        LogManager (const LogManager&) = delete;
        LogManager &operator=(const LogManager&) = delete;
        public:
            LogManager() {}
            ~LogManager() { shutdown(); }

            void init();

            void shutdown();

            inline bool isOpen() { return open; }
    };

} // End of Managers

} // End of Y

#endif /* logManager.hpp */