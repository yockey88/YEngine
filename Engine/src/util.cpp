#include "util.hpp"
#include "log.hpp"

#include <fstream>

namespace machy {
namespace util {

    const std::string readShaderFile(const std::string& path) {
        std::string line , hold = "";

        std::ifstream shader(path);
        MACHY_ASSERT(shader.is_open() , "Using Nonexistent Shader Paths");

        MACHY_TRACE("Loading Shader Program -> {}" , path);

        while (!shader.eof()) {
            getline(shader , line);
            hold += line + '\n';
        }

        return hold;
    }

    std::tuple<int , std::string> readSceneFile(const std::string& path) {
        int ret;
        std::string line , map = "";
        std::ifstream mapFile(path);

        MACHY_ASSERT(mapFile.is_open() , "Using Nonexistent Scene Path");

        MACHY_TRACE("Loading Map -> {}" , path);

        getline(mapFile , line);
        ret = std::stoi(line);
        while (!mapFile.eof()) {
            getline(mapFile , line);
            map += line;
        }

        return { ret , map };

    }

}
}