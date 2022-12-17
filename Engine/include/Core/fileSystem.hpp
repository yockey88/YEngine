#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "Game/scene.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"

#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <map>

namespace machy {
namespace core {

    class FileSystem {
        static void getPiecesOfLine(std::vector<std::string>& pieces , const std::string& line);
        static std::string getFileAsOneStr(const std::string& path);
        public:
            static std::shared_ptr<graphics::VertexArray> loadVertexFile(const std::string& path);
            static const std::shared_ptr<graphics::Shader> loadShaderFile(const std::string& vpath , const std::string& fpath);
            static std::shared_ptr<graphics::Material> loadMaterialFile(const std::string& path);

            static std::shared_ptr<game::Scene> loadScene(const std::string& path); 
            static void saveScene(std::shared_ptr<game::Scene> context);

            static void loadEntity(std::shared_ptr<game::Scene> context , const std::string& path);
            static void saveEntity(std::shared_ptr<game::Scene> context , game::Entity ent);

            static std::tuple<int , std::string> readSceneFile(const std::string& path);
    };

    

}
}

#endif