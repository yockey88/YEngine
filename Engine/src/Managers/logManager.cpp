#include "Managers/logManager.hpp"

#include <ctime>
#include <chrono>

namespace machy {
namespace managers {

    void LogManager::out(const std::string& data , bool error) {
        // auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // char* now = ctime(&time);
        // if (now[strlen(now) - 1] == '\n')
        //     now[strlen(now) - 1] = '\0';

        if (error) {
            std::cout << "{!!!}>>> [" << data << "] <<< []" << std::endl; // " << now << "
        } else {
            std::cout << "[$$$]>>> [" << data << "] <<<[]" << std::endl; //" << now << "
        }

        return;
    }

    void LogManager::file(const std::string& data , bool error) {
        // auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // char* now = ctime(&time);
        // if (now[strlen(now) - 1] == '\n')
        //     now[strlen(now) - 1] = '\0';

        if (error) {
            errsink.open(errlog , std::ios::app);
            errsink << "{!!!}>>> {" << data << "} <<<[]" << std::endl; //" << now << "
            errsink.close();
        } else {
            mainsink.open(mainlog , std::ios::app);
            mainsink << "[$$$]>>> [" << data << "] <<<[]" << std::endl; // " << now << "
            mainsink.close();
        }

        return;
    }

    void LogManager::log(const std::string& data) {
        file(data , false);

#ifdef MACHY_CONFIG_DEBUG
        out(data , false);
#endif

        return;
    }

    void LogManager::error(const std::string& data) {
        file(data , true);
        out(data , true);

        return;
    }

    void LogManager::shutdown() {
        file("Shutting Down Log Manager" , false);

        file("Closing Main Sink" , false);
        if (mainsink.is_open())
            mainsink.close();

        file("Closing Error Sink" , false);
        if (errsink.is_open())
            errsink.close();

        return;
    }

}
}