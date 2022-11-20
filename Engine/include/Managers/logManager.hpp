#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <iostream>
#include <queue>
#include <string>
#include <fstream>

namespace machy {

namespace managers {

    class LogManager {
        std::string mainlog = "main_logs/log.txt";
        std::string errlog = "err_logs/errLog.txt";
        std::queue<std::string> logQueue;
        std::ofstream mainsink;
        std::ofstream errsink;
        bool open;

        void out(const std::string &data , bool error);
        void file(const std::string &data , bool error);

        // Delete Copy Constructors
        LogManager (const LogManager&) = delete;
        LogManager &operator=(const LogManager&) = delete;
        public:
            LogManager() {}
            ~LogManager() { shutdown(); }

            void log(const std::string &data);
            void error(const std::string &data);

            void shutdown();

            std::queue<std::string>& getLogQueue() { return logQueue; }
            inline bool isOpen() { return open; }
    };

} // End of Managers

} // End of Y

#endif /* logManager.hpp */