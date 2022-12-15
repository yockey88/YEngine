#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <tuple>

namespace machy {
namespace util {

    const std::string readShaderFile(const std::string& path);

    std::tuple<int , std::string> readSceneFile(const std::string& path);

}
}

#endif