#include "core/fileSystem.hpp"
#include "machy.hpp"

#include "Game/sceneSerializer.hpp"

using js = nlohmann::json;

namespace machy {
namespace core {

    void FileSystem::getPiecesOfLine(std::vector<std::string>& pieces , const std::string& line) {
        std::string hold = "";
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                pieces.push_back(hold);
                hold = "";
            } else {
                hold += line[i];
            }
        }
        pieces.push_back(hold);

        return;
    }

    std::string FileSystem::getFileAsOneStr(const std::string& path) {
        std::string line , hold = "";
        std::ifstream shader(path);
        MACHY_ASSERT(shader.is_open() , "Using Nonexistent Shader Paths");

        while (!shader.eof()) {
            getline(shader , line);
            hold += line + '\n';
        }

        return hold;
    }

    std::shared_ptr<graphics::VertexArray> FileSystem::loadVertexFile(const std::string& path) {

        std::string line , hold = "" , meshName;

        std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();
        MACHY_CREATE_VERTEX_BUFFER(vb , float);

        std::ifstream file(path);
        MACHY_ASSERT(file.is_open() , "Using nonexistent Vertex paths");

        getline(file , line); // read name
        meshName = line;
        VA->setName(meshName);
        VA->setPath(path);

        getline(file , line); // read layout
        std::vector<uint32_t> layout;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') continue;
            std::string str(1 , line[i]);
            layout.push_back((uint32_t)std::stoi(str));
        }
        vb->setLayout(layout);

        getline(file , line); // read elements
        std::vector<uint32_t> elements;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') continue;
            std::string str(1 , line[i]);
            elements.push_back((uint32_t)std::stoi(str));
        }
        VA->setElements(elements);
        
        std::vector<float> vertElts;
        int lineNum = 0;
        while (!file.eof()) {
            getline(file , line);
            vertElts.clear();
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ',') {
                    vertElts.push_back(std::stof(hold));
                    hold = "";
                } else if (i == line.length()) {
                    vertElts.push_back(std::stof(hold));
                    hold = "";
                } else if (line[i] == 'z') {
                    vertElts.push_back(0.f);
                    hold = "";
                } else {
                    hold += line[i];
                }
            }

            vb->pushVertex(vertElts);  
            
            lineNum++;
        }
        VA->pushBuffer(std::move(vb));
        VA->upload();

        return VA;

    }

    const std::shared_ptr<graphics::Shader> FileSystem::loadShaderFile(const std::string& vpath , const std::string& fpath) {

        std::string vShaderStr = getFileAsOneStr(vpath);
        std::string fShaderStr = getFileAsOneStr(fpath);

        std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderStr , fShaderStr);

        return shader;
    }

    std::shared_ptr<graphics::Material> FileSystem::loadMaterialFile(const std::string& path) {

        std::string line , hold = "";

        std::ifstream file(path);
        MACHY_ASSERT(file.is_open() , "Using Non Existent Material Files");

        std::map<std::string , std::string> paths;
        SceneSerializer serializer;
        std::string name = serializer.deserializeMat(paths , path);
        MACHY_ASSERT(paths.size() > 0 , "Error Deserializing Material");

        std::string vertP = paths["vert"];
        std::string fragP = paths["frag"];
        std::string textrP = paths["texture"];

        MACHY_ASSERT((vertP != "null" && fragP != "null") , "Cannot Have Null Shader Paths Into Material File");

        std::shared_ptr<graphics::Shader> shader = loadShaderFile(vertP , fragP);

        std::shared_ptr<graphics::Material> mat;
        if (textrP != "null") {
            std::shared_ptr<graphics::Texture> text = std::make_shared<graphics::Texture>(textrP);
            mat = std::make_shared<graphics::Material>(shader);
        } else {
            mat = std::make_shared<graphics::Material>(shader);
        }
        mat->setName(name);
        mat->setPath(path);

        return mat;
    }

    std::shared_ptr<game::Scene> FileSystem::loadScene(const std::string& path) {

        MACHY_INFO("Loading Scene -> <{}>" , path);
        std::shared_ptr<game::Scene> ret = std::make_shared<game::Scene>();
        SceneSerializer serializer(ret);
        ret = serializer.deserialize(path);

        return ret;
    }

    void FileSystem::saveScene(std::shared_ptr<game::Scene> context) {
        
        SceneSerializer serializer(context);
        if (context->getName() != "{Blank Scene}") {
            std::string path = "resources\\scenes\\" + context->getName() + ".json";
            MACHY_INFO("Saving Scene -> <{}>" , path);
            serializer.serialize(path);
        }

        return;
    }

    void FileSystem::loadEntity(std::shared_ptr<game::Scene> context , const std::string& path) {

        SceneSerializer serializer(context);
        serializer.deserializeEnt(path);

        return;
    }
            
    void FileSystem::saveEntity(std::shared_ptr<game::Scene> context , game::Entity ent) {

        SceneSerializer serializer(context);
        serializer.serializeEnt(ent);

        return;
    }

    std::tuple<int , std::string> FileSystem::readSceneFile(const std::string& path) {
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