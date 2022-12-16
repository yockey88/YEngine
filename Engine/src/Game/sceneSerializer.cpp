#include "Game/sceneSerializer.hpp"
#include "Game/scene.hpp"
#include "Game/Entity/entity.hpp"
#include "Game/Entity/entityComponents.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>

namespace machy {

    void GameSceneSerializer::serializeEnt(game::Entity& entity , const std::string& filepath) {

        js json;

        if (entity.HasComponent<game::EntityID>()) {
            auto& id = context->Entts().get<game::EntityID>(entity.get());
            json["ID"] = id.name;
        }

        if (entity.HasComponent<game::PositionComponent>()) {
            serializeEntPos(entity , json);
        }  
        if (entity.HasComponent<game::RenderComponent>()) {
            serializeEntSprite(entity , json);
        }   
        if (entity.HasComponent<game::CameraComponent>()) {
            serializeCamPos(entity , json);
        }

        std::ofstream file(filepath);
        if (!file.is_open()) {
            MACHY_WARN("Scene File Not Found");
            return;
        }

        file << std::setw(4) << json << std::endl;
        file.close();

        return;
    }

    void GameSceneSerializer::serializeEntPos(game::Entity& entity , js& json) {

        auto& id = context->Entts().get<game::EntityID>(entity.get());
        auto& pos = context->Entts().get<game::PositionComponent>(entity.get());

        json["position"] = { pos.pos.x , pos.pos.y , pos.pos.z };
        json["rotation"] = { pos.rotation.x , pos.rotation.y , pos.rotation.z };
        json["size"] = { pos.size.x , pos.size.y , pos.size.z };

        return;
    }

    void GameSceneSerializer::serializeEntSprite(game::Entity& entity , js& json) {

        auto& id = context->Entts().get<game::EntityID>(entity.get());
        auto& sprite = context->Entts().get<game::RenderComponent>(entity.get());

        json["sprite"] = { sprite.color.r , sprite.color.g , sprite.color.b };

        if (sprite.meshName != "Skeleton")
            json["meshname"] = sprite.meshName;
        if (sprite.matName.substr(0 , 3) != "Mat")
            json["matname"] = sprite.matName;

        return;
    }

    void GameSceneSerializer::serializeCamPos(game::Entity& entity , js& json) {

        auto& id = context->Entts().get<game::EntityID>(entity.get());
        auto& cam = context->Entts().get<game::CameraComponent>(entity.get());

        json["camera"] = { cam.cameraPos.x , cam.cameraPos.y , cam.cameraPos.z , cam.cameraRotation };

        return;
    }

    void GameSceneSerializer::deserializeEnt(const std::string& filepath) {
        
        std::ifstream file(filepath);
        MACHY_ASSERT(file.is_open() , "Could Not Open File");

        game::Entity entity;
        js json;
        file >> json;

        if (json.contains("ID")) {
            entity = context->createEnt(json["ID"].get<std::string>());
            entity.setContext(context.get());
        }
        if (json.contains("/position"_json_pointer)) {
            MACHY_ASSERT((json.contains("/position/0"_json_pointer) && json.contains("/position/1"_json_pointer) && json.contains("/position/2"_json_pointer)
                            && json.contains("/size"_json_pointer) && json.contains("/size/0"_json_pointer) && json.contains("/size/1"_json_pointer) && json.contains("/size/2"_json_pointer)
                            && json.contains("/rotation"_json_pointer) && json.contains("/rotation/0"_json_pointer) && json.contains("/rotation/1"_json_pointer) && json.contains("/rotation/2"_json_pointer)),
                            "Entity Save File Corrupt");
            deserializeEntPos(entity , json);
        }
        if (json.contains("/sprite"_json_pointer)) {
            MACHY_ASSERT((json.contains("/sprite/0"_json_pointer) && json.contains("/sprite/1"_json_pointer) && json.contains("/sprite/2"_json_pointer)) , "Entity Save File Corrupt");
            deserializeEntSprite(entity , json);
        }
        if (json.contains("/camera"_json_pointer)) {
            MACHY_ASSERT((json.contains("/camera"_json_pointer) && json.contains("/camera/0"_json_pointer) && json.contains("/camera/1"_json_pointer)
                            && json.contains("/camera/2"_json_pointer) && json.contains("/camera/3"_json_pointer)) , "Entity Save File Corrupt");
            deserializeCamPos(entity , json);
        }

        return;
    }

    void GameSceneSerializer::deserializeEntPos(game::Entity& entity , js& json) {

        auto& pos = entity.AddComponent<game::PositionComponent>();
        pos.pos.x = json["/position/0"_json_pointer].get<float>();
        pos.pos.y = json["/position/1"_json_pointer].get<float>(); 
        pos.pos.z = json["/position/2"_json_pointer].get<float>();

        pos.rotation.x = json["/rotation/0"_json_pointer].get<float>();
        pos.rotation.y = json["/rotation/1"_json_pointer].get<float>();
        pos.rotation.z = json["/rotation/2"_json_pointer].get<float>();

        pos.size.x = json["/size/0"_json_pointer].get<float>();
        pos.size.y = json["/size/1"_json_pointer].get<float>(); 
        pos.size.z = json["/size/2"_json_pointer].get<float>();

        return;
    }

    void GameSceneSerializer::deserializeEntSprite(game::Entity& entity , js& json) {

        auto& sprite = entity.AddComponent<game::RenderComponent>();
        auto& meshes = context->getVertLib();
        auto& shaders = context->getShaderLib();
        auto& mats = context->getMatLib();

        sprite.color.r = json["/sprite/0"_json_pointer].get<float>();
        sprite.color.g = json["/sprite/1"_json_pointer].get<float>();
        sprite.color.b = json["/sprite/2"_json_pointer].get<float>();

        if (json.contains("/meshname"_json_pointer)) {
            std::string meshname = json["/meshname"_json_pointer].get<std::string>();
            if (context->getVertLib().exists(meshname)) {
                sprite.skeleton = meshes.get(meshname);
                sprite.meshName = meshname;
            } else {
                MACHY_WARN("[ENTITY LOAD ERROR] Requested Mesh Not Found In Context | Default Loaded Instead");
                sprite.skeleton = meshes.get("Skeleton");
                sprite.meshName = "Skeleton";
            }
        } else {
            sprite.skeleton = meshes.get("Skeleton");
            sprite.meshName = "Skeleton";
        }

        if (json.contains("/matname"_json_pointer)) {
            std::string matname = json["/matname"_json_pointer].get<std::string>();
            if (context->getMatLib().exists(matname)) {
                sprite.material = context->getMatLib().get(matname);
                sprite.matName = matname;
            } else {
                MACHY_WARN("[ENTITY LOAD ERROR] Requested Sprite Not Found In Context | Default Loaded Instead");
                std::shared_ptr<graphics::Material> newMat = std::make_shared<graphics::Material>(shaders.get("Basic"));
                int num = (int)mats.getAllAssets().size() + 1;
                std::string name = "Mat" + std::to_string(num);
                mats.load(name , newMat);
                sprite.material = newMat;
                sprite.matName = name;
            }
        } else {
            std::shared_ptr<graphics::Material> newMat = std::make_shared<graphics::Material>(shaders.get("Basic"));
            int num = (int)mats.getAllAssets().size() + 1;
            std::string name = "Mat" + std::to_string(num);
            mats.load(name , newMat);
            sprite.material = newMat;
            sprite.matName = name;
        }

        sprite.material->setUniformValue("inColor" , sprite.color);

        return;
    }

    void GameSceneSerializer::deserializeCamPos(game::Entity& entity , js& json) {

        auto& cam = entity.AddComponent<game::CameraComponent>();
        cam.camera->setHeight(json["/camera/2"_json_pointer].get<float>());
        cam.cameraPos.x = json["/camera/0"_json_pointer].get<float>();
        cam.cameraPos.y = json["/camera/1"_json_pointer].get<float>();
        cam.cameraPos.z = cam.camera->getHeight();
        cam.cameraRotation = json["/camera/3"_json_pointer].get<float>();
        cam.camera->setViewMat(cam.cameraPos , cam.cameraRotation);

        return;
    }

    void GameSceneSerializer::serialize(const std::string& filepath) {

        std::ofstream file(filepath);
        MACHY_ASSERT(file.is_open() , "Scene File Not Found");

        js json;

        json["scene"] = context->getName();
        json["scenepath"] = context->getPath();
        json["numents"] = context->getNumEnts();
        json["numshaders"] = context->getShaderLib().getAllAssets().size();
        json["numtextures"] = context->getTextureLib().getAllAssets().size();

        int i = 0;
        std::vector<std::string> entPaths;
        context->Entts().each([&] (auto entity) {
                game::Entity ent = context->getEntity(entity);
                auto& id = context->Entts().get<game::EntityID>(ent.get());
                std::string path = "resources/scenes/entities/" + id.name + ".json";
                std::string jsTag = "entpath" + std::to_string(i); i++; 
                json[jsTag.c_str()] = path;

                entPaths.push_back(path);
                serializeEnt(ent , path);
            });

        file << std::setw(4) << json << std::endl;
        file.close();

        return;
    }

    void GameSceneSerializer::serializeRuntime(const std::string& filepath) {

        MACHY_ASSERT(false , "Unimplemented Function! | Future API :)");

        return;
    }

    std::shared_ptr<game::Scene> GameSceneSerializer::deserialize(const std::string& filepath) {

        context = std::make_shared<game::Scene>();

        std::ifstream file(filepath , std::ios::app);
        MACHY_ASSERT(file.is_open() , "Could Not open Scene File");

        js json;
        file >> json;

        int numEntsInScene = -1;
        
        if (json.contains("scene")) {
            context->setSceneName(json["scene"]);
            MACHY_ASSERT(json.contains("scenepath") , "Scene File is Missing Data");
            context->setScenePath(json["scenepath"]);
            if (json.contains("numents"))
                numEntsInScene = json["numents"].get<int>();
        }

        if (numEntsInScene > 0) {
            
            for (int i = 0; i < numEntsInScene; i++) {
                std::string tag = "entpath" + std::to_string(i);
                std::string path = json[tag].get<std::string>();

                deserializeEnt(path);
                MACHY_INFO("Successful File Read");
            }

        }

        return context;
    }

    void GameSceneSerializer::deserializeRuntime(const std::string& filepath) {

        MACHY_ASSERT(false , "Unimplemented Function! | Future API :)");

        return;
    }

}