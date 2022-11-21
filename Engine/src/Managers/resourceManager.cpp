#include "Managers/resourceManager.hpp"
#include "machy.hpp"
#include "log.hpp"
#include <fstream>

namespace machy {
namespace managers {

    void ResourceManager::split(const std::string& line , std::string pieces[]) {
        int numPieces = 0;
        std::string hold = "";

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                pieces[0] = hold;
                pieces[1] = line.substr(i + 1 , line.length() - (i + 1));
                return;
            }

            hold += line[i];
        }

        return;
    }

    bool ResourceManager::init(/* const std::string& loadpath */) {
        // std::string line;
        // std::string pieces[2];
        // std::ifstream loadfile(loadpath);
        // MACHY_TRACE("Loading {}" , loadpath)
        // if (!loadfile.is_open()) {
        //     MACHY_FATAL("Application Loadfile is Corrupt" , loadpath);
        //     return false;
        // }
        
        // while (!loadfile.eof()) {
        //     getline(loadfile , line);
        //     split(line , pieces);
        //     MACHY_INFO("[{} | {}]" , pieces[0] , pieces[1]);
        //     paths[pieces[0]] = pieces[1];
        // }

        return true;
    }

}
}