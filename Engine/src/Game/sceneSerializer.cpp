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
            json["ID"] = { id.ID , id.name };
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
        json["size"] = { pos.size.x , pos.size.y , pos.size.z };

        return;
    }

    void GameSceneSerializer::serializeEntSprite(game::Entity& entity , js& json) {

        auto& id = context->Entts().get<game::EntityID>(entity.get());
        auto& sprite = context->Entts().get<game::RenderComponent>(entity.get());

        json["color"] = { sprite.color.r , sprite.color.g , sprite.color.b };

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

        if (json.contains("/ID"_json_pointer)) {
            entity = context->createEnt(json["/ID/1"_json_pointer]);
            entity.setContext(context.get());

            auto& id = context->Entts().get<game::EntityID>(entity.get());
            id.ID = json["/ID/0"_json_pointer].get<int>();
        }
        if (json.contains("/position"_json_pointer)) {
            deserializeEntPos(entity , json);
        }
        if (json.contains("/color"_json_pointer)) {
            deserializeEntSprite(entity , json);
        }
        if (json.contains("/camera"_json_pointer)) {
            deserializeCamPos(entity , json);
        }

        return;
    }

    void GameSceneSerializer::deserializeEntPos(game::Entity& entity , js& json) {

        auto& pos = entity.AddComponent<game::PositionComponent>();
        pos.pos.x = json["/position/0"_json_pointer].get<float>();
        pos.pos.y = json["/position/1"_json_pointer].get<float>(); 
        pos.pos.z = json["/position/2"_json_pointer].get<float>();

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

        sprite.skeleton = meshes.get("Skeleton");

        int num = (int)mats.getAllAssets().size() + 1;
        std::string name = "Mat" + std::to_string(num);
        std::shared_ptr<graphics::Material> newMat = std::make_shared<graphics::Material>(shaders.get("Basic"));
        mats.load(name , newMat);

        sprite.material = newMat;

        sprite.color.r = json["/color/0"_json_pointer].get<float>();
        sprite.color.g = json["/color/1"_json_pointer].get<float>();
        sprite.color.b = json["/color/2"_json_pointer].get<float>();

        // sprite

        sprite.material->setUniformValue("inColor" , sprite.color);

        return;
    }

    void GameSceneSerializer::deserializeCamPos(game::Entity& entity , js& json) {

        auto& cam = entity.AddComponent<game::CameraComponent>();
        cam.camera->setHeight(json["/camera/2"_json_pointer].get<float>());
        cam.cameraPos.x = json["/camera/0"_json_pointer].get<float>();
        cam.cameraPos.y = json["/camera/1"_json_pointer].get<float>();
        cam.cameraPos.z = json["/camera/2"_json_pointer].get<float>();
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

        auto& verts = context->getVertLib();
        auto& shaders = context->getShaderLib();
        auto& materials = context->getMatLib();

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
            context->setScenePath(json["scenepath"]);
            if (json.contains("numents"))
                numEntsInScene = json["numents"].get<int>();
        }

        if (numEntsInScene > 0) {
            
            for (int i = 0; i < numEntsInScene; i++) {
                std::string tag = "entpath" + std::to_string(i);
                std::string path = json[tag].get<std::string>();

                deserializeEnt(path);
            }

        }

        return context;
    }

    void GameSceneSerializer::deserializeRuntime(const std::string& filepath) {

        MACHY_ASSERT(false , "Unimplemented Function! | Future API :)");

        return;
    }

}