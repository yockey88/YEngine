#include "Game/sceneSerializer.hpp"
#include "Core/fileSystem.hpp"
#include "Game/scene.hpp"
#include "Game/Entity/entity.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "box2d/box2d.h"

#include <iostream>
#include <iomanip>
#include <fstream>

namespace machy {

    js SceneSerializer::getJson(const std::string& path) {

        std::ifstream file(path);
        MACHY_ASSERT(file.is_open() , "Could Not Open File");

        js json;
        file >> json;

        return json;
    }

    void SceneSerializer::serializeEnt(game::Entity& entity) {

        js json;

        if (entity.HasComponent<game::EntityID>()) {
            auto& id = entity.GetComponent<game::EntityID>();
            json["ID"] = id.name;
        }

        if (entity.HasComponent<game::PositionComponent>()) {
            serializeEntPos(entity , json);
        }  
        if (entity.HasComponent<game::RenderComponent>()) {
            serializeEntSprite(entity , json);
        }
        if (entity.HasComponent<game::PhysicsBody2DComponent>()) {
            serializeEntPhysics2DPos(entity , json);
        }   
        if (entity.HasComponent<game::CameraComponent>()) {
            serializeCamPos(entity , json);
        }

        std::ofstream file(entity.getPath());
        if (!file.is_open()) {
            MACHY_WARN("Scene File Not Found");
            return;
        }

        file << std::setw(4) << json << std::endl;
        file.close();

        return;
    }

    void SceneSerializer::serializeEntPos(game::Entity& entity , js& json) {

        auto& id = entity.GetComponent<game::EntityID>();
        auto& pos = entity.GetComponent<game::PositionComponent>();

        json["position"] = { pos.pos.x , pos.pos.y , pos.pos.z };
        json["rotation"] = { pos.rotation.x , pos.rotation.y , pos.rotation.z };
        json["size"] = { pos.size.x , pos.size.y , pos.size.z };

        return;
    }

    void SceneSerializer::serializeEntSprite(game::Entity& entity , js& json) {

        auto& id = entity.GetComponent<game::EntityID>();
        auto& sprite = entity.GetComponent<game::RenderComponent>();

        json["color"] = { sprite.color.r , sprite.color.g , sprite.color.b };
        json["meshname"] = sprite.skeleton->getName();

        if ((sprite.material->getPath() == "") || (sprite.material->getPath()[0] != 'r')) {
            json["matpath"] = "resources/assets/materials/basic.json";
        } else {
            json["matpath"] = sprite.material->getPath();
        }

        return;
    }

    void SceneSerializer::serializeEntPhysics2DPos(game::Entity& entity , js& json) {

        auto& physics = entity.GetComponent<game::PhysicsBody2DComponent>();

        json["physics2D"] = (int)physics.type;
        json["density"] = physics.density;
        json["friction"] = physics.friction;
        json["restitution"] = physics.restitution;
        json["restitutionThreshold"] = physics.restitutionThreshold;

        return;
    }

    void SceneSerializer::serializeCamPos(game::Entity& entity , js& json) {

        auto& id = entity.GetComponent<game::EntityID>();
        auto& cam = entity.GetComponent<game::CameraComponent>();

        json["camera"] = { cam.cameraPos.x , cam.cameraPos.y , cam.cameraPos.z , cam.cameraRotation };

        return;
    }

    void SceneSerializer::deserializeEnt(const std::string& filepath) {
        
        js json = getJson(filepath);

        game::Entity entity;

        if (json.contains("ID")) {
            entity = context->createEnt(json["ID"].get<std::string>());
            entity.setContext(context.get());
        }
        if (json.contains("/position"_json_pointer)) {
            MACHY_ASSERT((json.contains("/position/0"_json_pointer) && json.contains("/position/1"_json_pointer) && json.contains("/position/2"_json_pointer)
                            && json.contains("/size"_json_pointer) && json.contains("/size/0"_json_pointer) && json.contains("/size/1"_json_pointer) && json.contains("/size/2"_json_pointer)
                            && json.contains("/rotation"_json_pointer) && json.contains("/rotation/0"_json_pointer) && json.contains("/rotation/1"_json_pointer) && json.contains("/rotation/2"_json_pointer)),
                            "Entity Save File Corrupt | Missing Position Data");
            deserializeEntPos(entity , json);
        }
        if (json.contains("/color"_json_pointer)) {
            MACHY_ASSERT((json.contains("/color/0"_json_pointer) && json.contains("/color/1"_json_pointer) && json.contains("/color/2"_json_pointer)
                            && json.contains("/meshname"_json_pointer) && json.contains("/matpath"_json_pointer)) , 
                            "Entity Save File Corrupt | Missing Render Data");
            deserializeEntSprite(entity , json);
        }
        if (json.contains("/camera"_json_pointer)) {
            MACHY_ASSERT((json.contains("/camera"_json_pointer) && json.contains("/camera/0"_json_pointer) && json.contains("/camera/1"_json_pointer)
                            && json.contains("/camera/2"_json_pointer) && json.contains("/camera/3"_json_pointer)) , 
                            "Entity Save File Corrupt | Missing Camera Data");
            deserializeCamPos(entity , json);
        }
        if (json.contains("/physics2D"_json_pointer)) {
            MACHY_ASSERT((json.contains("/density"_json_pointer) && json.contains("/friction"_json_pointer) && json.contains("/restitution"_json_pointer)
                            && json.contains("/restitutionThreshold"_json_pointer)) , "Entity Save File Corrupt | Missing Physics Data");
            deserializeEntPhysics2D(entity , json);
        }

        return;
    }

    void SceneSerializer::deserializeEntPos(game::Entity& entity , js& json) {

        auto& pos = entity.GetComponent<game::PositionComponent>();
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

    void SceneSerializer::deserializeEntSprite(game::Entity& entity , js& json) {

        auto& sprite = entity.AddComponent<game::RenderComponent>();
        auto& meshes = context->getVertLib();

        sprite.color.r = json["/color/0"_json_pointer].get<float>();
        sprite.color.g = json["/color/1"_json_pointer].get<float>();
        sprite.color.b = json["/color/2"_json_pointer].get<float>();

        std::string meshname = json["/meshname"_json_pointer].get<std::string>();
        if (context->getVertLib().exists(meshname)) {
            sprite.skeleton = meshes.get(meshname);
            sprite.meshName = meshname;
        } else {
            sprite.skeleton = meshes.get("Skeleton");
            sprite.meshName = "Skeleton";
        }

        std::string matpath = json["/matpath"_json_pointer].get<std::string>();
        sprite.material = core::FileSystem::loadMaterialFile(matpath);

        sprite.material->setUniformValue("inColor" , sprite.color);

        return;
    }

    void SceneSerializer::deserializeEntPhysics2D(game::Entity& entity , js& json) {

        auto& physics = entity.AddComponent<game::PhysicsBody2DComponent>();
        int typeRaw = json["/physics2D"_json_pointer].get<int>();
        physics.type = (game::PhysicsBody2DComponent::PhysicsType)typeRaw;
        physics.friction = json["/friction"_json_pointer].get<float>();
        physics.density = json["/density"_json_pointer].get<float>();
        physics.restitution = json["/restitution"_json_pointer].get<float>();
        physics.restitutionThreshold = json["/restitutionThreshold"_json_pointer].get<float>();

        return;
    }

    void SceneSerializer::deserializeCamPos(game::Entity& entity , js& json) {

        auto& cam = entity.AddComponent<game::CameraComponent>();
        cam.camera->setHeight(json["/camera/2"_json_pointer].get<float>());
        cam.cameraPos.x = json["/camera/0"_json_pointer].get<float>();
        cam.cameraPos.y = json["/camera/1"_json_pointer].get<float>();
        cam.cameraPos.z = cam.camera->getHeight();
        cam.cameraRotation = json["/camera/3"_json_pointer].get<float>();
        cam.camera->setViewMat(cam.cameraPos , cam.cameraRotation);

        return;
    }

    std::string SceneSerializer::deserializeMat(std::map<std::string , std::string>& paths , const std::string& path) {
        
        js json = getJson(path);

        MACHY_ASSERT((json.contains("/Matname"_json_pointer) && json.contains("/Shaders"_json_pointer) &&
                        json.contains("/Texture"_json_pointer)) , "Material File Corrupt");

        paths["texture"] = json["/Texture"_json_pointer].get<std::string>();

        std::filesystem::path fsPath = json["/Shaders/0"_json_pointer].get<std::string>();
        if (fsPath.extension().string() == ".vert") {
            paths["vert"] = json["/Shaders/0"_json_pointer].get<std::string>();
            paths["frag"] = json["/Shaders/1"_json_pointer].get<std::string>();
        } else {
            paths["frag"] = json["/Shaders/0"_json_pointer].get<std::string>();
            paths["vert"] = json["/Shaders/1"_json_pointer].get<std::string>();
        }

        std::string name = json["/Matname"_json_pointer].get<std::string>();

        return name;
    }

    void SceneSerializer::serialize(const std::string& filepath) {

        std::ofstream file(filepath);
        MACHY_ASSERT(file.is_open() , "Scene File Not Found");

        js json;

        json["scene"] = context->getName();
        json["scenepath"] = context->getPath();
        json["numents"] = context->getNumEnts();
        json["nummeshes"] = context->getVertLib().getAllAssets().size();

        int i = 0;
        for (auto& vert : context->getVertLib().getAllAssets()) {
            std::string tag = "meshpath" + std::to_string(i);
            json[tag] = vert.second->getPath();
            i++;
        }

        int j = 0;
        std::vector<std::string> entPaths;
        context->Entts().each([&] (auto entity) {
                game::Entity ent = context->getEntity(entity);
                auto& id = context->Entts().get<game::EntityID>(ent.get());
                std::string path = "resources/scenes/entities/" + id.name + ".json";
                ent.setPath(path);
                std::string jsTag = "entpath" + std::to_string(j); j++; 
                json[jsTag.c_str()] = path;

                entPaths.push_back(path);
                serializeEnt(ent);
            });

        file << std::setw(4) << json << std::endl;
        file.close();

        return;
    }

    void SceneSerializer::serializeRuntime(const std::string& filepath) {

        MACHY_ASSERT(false , "Unimplemented Function! | Future API :)");

        return;
    }

    std::shared_ptr<game::Scene> SceneSerializer::deserialize(const std::string& filepath) {

        context = std::make_shared<game::Scene>();

        js json = getJson(filepath);

        int numEntsInScene = -1;
        int numMeshesInLib = -1;
        
        if (json.contains("scene")) {
            context->setSceneName(json["scene"]);
            MACHY_ASSERT(json.contains("scenepath") , "Scene File is Missing Data");
            context->setScenePath(json["scenepath"]);
            if (json.contains("numents"))
                numEntsInScene = json["numents"].get<int>();
            if (json.contains("nummeshes"))
                numMeshesInLib = json["nummeshes"].get<int>();
        }

        if (numMeshesInLib > 0) {
            std::shared_ptr<graphics::VertexArray> VA;
            for (int i = 0; i < numMeshesInLib; i++) {
                std::string tag = "meshpath" + std::to_string(i);
                std::string path = json[tag].get<std::string>();

                VA = core::FileSystem::loadVertexFile(path);
                if (!context->getVertLib().exists(VA->getName()))
                    context->getVertLib().load(VA->getName() , VA);
            }
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

    void SceneSerializer::deserializeRuntime(const std::string& filepath) {

        MACHY_ASSERT(false , "Unimplemented Function! | Future API :)");

        return;
    }

}