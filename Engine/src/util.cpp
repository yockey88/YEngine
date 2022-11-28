#include "util.hpp"
#include "log.hpp"

#include <fstream>

namespace machy {
namespace util {

    const std::string readShaderFile(const std::string& path) {
        std::string line;
        std::string hold = "";

        std::ifstream shader(path);

        MACHY_ASSERT(shader.is_open() , "Using Nonexistent Shader Paths");

        while (!shader.eof()) {
            getline(shader , line);
            hold += line + '\n';
        }

        return hold;
    }

}
}