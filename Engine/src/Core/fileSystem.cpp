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

    std::shared_ptr<graphics::VertexArray> FileSystem::createBasicMesh() {

        std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();
        VA->setName("Skeleton");

        MACHY_CREATE_VERTEX_BUFFER(vb , float);
        vb->pushVertex({  0.5 ,  0.5 , 0.0 });
        vb->pushVertex({  0.5 , -0.5 , 0.0 });
        vb->pushVertex({ -0.5 , -0.5 , 0.0 });
        vb->pushVertex({ -0.5 ,  0.5 , 0.0 });
        vb->setLayout({ 3 });

        VA->setElements({ 0 , 3 , 1 , 1 , 3 , 2 });
        VA->pushBuffer(std::move(vb));
        VA->upload();

        return VA;

    }

    std::shared_ptr<graphics::VertexArray> FileSystem::createTexturedMesh() {

        std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();
        VA->setName("TexturedSkeleton");
        
        {
            MACHY_CREATE_VERTEX_BUFFER(vb , float);
            vb->pushVertex({  0.5 ,  0.5 , 0.0 });
            vb->pushVertex({  0.5 , -0.5 , 0.0 });
            vb->pushVertex({ -0.5 , -0.5 , 0.0 });
            vb->pushVertex({ -0.5 ,  0.5 , 0.0 });
            vb->setLayout({ 3 });
            VA->pushBuffer(std::move(vb));
        }
        {
            MACHY_CREATE_VERTEX_BUFFER(vb , short);
            vb->pushVertex({ 1 , 1 });
            vb->pushVertex({ 1 , 0 });
            vb->pushVertex({ 0 , 0 });
            vb->pushVertex({ 0 , 1 });
            vb->setLayout({ 2 });
            VA->pushBuffer(std::move(vb));
        }
        VA->setElements({ 0 , 3 , 1 , 1 , 3 , 2 });
        VA->upload();

        return VA;

    }

    std::shared_ptr<graphics::VertexArray> FileSystem::createTexturedMesh(const glm::vec4& uvsR , const glm::vec4& uvsL) {

        std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();
        VA->setName("TexturedSkeleton");
        
        MACHY_CREATE_VERTEX_BUFFER(vb , float);
        vb->pushVertex({  0.5 ,  0.5 , 0.0 , uvsR.x , uvsR.y });
        vb->pushVertex({  0.5 , -0.5 , 0.0 , uvsR.z , uvsR.w });
        vb->pushVertex({ -0.5 , -0.5 , 0.0 , uvsL.x , uvsL.y });
        vb->pushVertex({ -0.5 ,  0.5 , 0.0 , uvsL.z , uvsL.w });
        vb->setLayout({ 3 , 2 });
        VA->pushBuffer(std::move(vb));
        VA->setElements({ 0 , 3 , 1 , 1 , 3 , 2 });
        VA->upload();

        return VA;

    }

    const std::shared_ptr<graphics::Shader> FileSystem::loadShaderFile(const std::string& vpath , const std::string& fpath) {

        std::string vShaderStr = getFileAsOneStr(vpath);
        std::string fShaderStr = getFileAsOneStr(fpath);

        std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderStr , fShaderStr);

        return shader;
    }

    const std::shared_ptr<graphics::Material> FileSystem::loadTexturedMaterialFile(const std::string& path) {

        std::string line , hold = "";

        std::ifstream file(path);
        MACHY_ASSERT(file.is_open() , "Using Non Existent Material Files");

        std::map<std::string , std::string> paths;
        SceneSerializer serializer;
        std::string name = serializer.deserializeTexturedMat(paths , path);
        MACHY_ASSERT(paths.size() > 0 , "Error Deserializing Material");

        std::string vertP = paths["vert"];
        std::string fragP = paths["frag"];
        std::string  textrP = paths["texture"];

        MACHY_ASSERT((vertP != "null" && fragP != "null") , "Cannot Have Null Shader Paths Into Material File");

        std::shared_ptr<graphics::Shader> shader = loadShaderFile(vertP , fragP);
        std::shared_ptr<graphics::Texture> text = std::make_shared<graphics::Texture>(textrP);
        std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(shader , text);    
        mat->setName(name);
        mat->setPath(path);

        return mat;

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

        MACHY_ASSERT((vertP != "null" && fragP != "null") , "Cannot Have Null Shader Paths Into Material File");

        std::shared_ptr<graphics::Shader> shader = loadShaderFile(vertP , fragP);

        std::shared_ptr<graphics::Material> mat;
        mat = std::make_shared<graphics::Material>(shader);    
        mat->setName(name);
        mat->setPath(path);

        return mat;
    }

    std::shared_ptr<game::Scene> FileSystem::loadScene(const std::string& path) {

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