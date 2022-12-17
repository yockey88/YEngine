#include "Game/scene.hpp"
#include "Core/fileSystem.hpp"
#include "Game/Entity/scriptedEntity.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "machy.hpp"

static std::string sceneBaseMeshPath = "resources/assets/meshes/skeleton.csv";
static std::string sceneTexMeshPath = "resources/assets/meshes/textrSkeleton.csv";

static std::string baseCamVShader = "resources/assets/shaders/basic_camera_shader.vert";
static std::string baseCamFShader = "resources/assets/shaders/basic_camera_shader.frag";
static std::string baseCamCShader = "resources/assets/shaders/basic_circle.frag";
static std::string baseCamTVShader = "resources/assets/shaders/basic_camera_shader_texture.vert";
static std::string baseCamTFShader = "resources/assets/shaders/basic_camera_shader_texture.frag";

namespace machy {
namespace game {

    Scene::Scene() : numEnts(0) , totalEntsCreated(0) , playing(true) , name("{Blank Scene}")  {
        Entity nullEnt;
        nullEnt.handle = entt::null;

        entities[0] = nullEnt;

        std::shared_ptr<graphics::VertexArray> VA = core::FileSystem::loadVertexFile(sceneBaseMeshPath);
        std::shared_ptr<graphics::VertexArray> tVA = core::FileSystem::loadVertexFile(sceneTexMeshPath);
        MeshLib.load(VA->getName() , VA);
        MeshLib.load(tVA->getName() , tVA);

        std::shared_ptr<graphics::Shader> shader = core::FileSystem::loadShaderFile(baseCamVShader , baseCamFShader);
        std::shared_ptr<graphics::Shader> circle = core::FileSystem::loadShaderFile(baseCamVShader , baseCamCShader);
        std::shared_ptr<graphics::Shader> tshader = core::FileSystem::loadShaderFile(baseCamTVShader , baseCamTFShader);
        ShaderLib.load("Basic" , shader);
        ShaderLib.load("Circle" , circle);
        ShaderLib.load("Texture" , tshader);
    }

    Scene::~Scene() {
        entRegistry.each([&] (auto entity) { entRegistry.destroy(entity); });
    }

    void Scene::updateFromEditor() {

        return;
    }

    void Scene::updateRuntime() {
        entRegistry.view<NativeScript>().each([=](auto entity , auto& script) {
                if (script.instance == nullptr) {
                    script.instance = script.BindScript();
                    script.instance->entity = { entity };
                    script.instance->entity.setContext(this);
                    script.instance->onCreation();
                }
                script.instance->onUpdate();
            });

        return;
    }

    void Scene::render()  {
        
        auto sprites = entRegistry.view<RenderComponent , PositionComponent>();
        auto cameras = entRegistry.view<CameraComponent>();

        if (playing) {
            if (cameras.size() > 0)
                for (auto& ent : cameras)
                    if (entRegistry.valid(ent)) {
                        auto& cam = cameras.get<CameraComponent>(ent);
                        MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , cam.camera));
                    }
        }

        for (const auto ent : sprites) {
            if (entRegistry.valid(ent)) {
                Entity entity{ ent };
                entity.setContext(this);
                if (entity.HasComponent<RenderComponent>() && entity.HasComponent<RenderComponent>()){
                    auto& renderable = sprites.get<RenderComponent>(ent);
                    auto& position = sprites.get<PositionComponent>(ent);

                    glm::mat4 model = position.getModel();
                    if (renderable.skeleton.get() != nullptr && renderable.material.get() != nullptr)
                        MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , renderable.skeleton , renderable.material , model));
                }
            }
        }

        if (playing) {
            if (cameras.size() > 0)
                for (auto& ent : cameras)
                    if (entRegistry.valid(ent)) {
                        MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));
                    }
        }

        return;
    }

    Entity Scene::createEnt(const std::string& name) {
        Entity newEnt(entRegistry.create());
        newEnt.setContext(this);

        auto& id = entRegistry.emplace<EntityID>(newEnt.get());
        auto& pos = entRegistry.emplace<PositionComponent>(newEnt.get());

        numEnts++;
        totalEntsCreated++;

        id.ID = totalEntsCreated;
        id.name = name;

        entities[id.ID] = newEnt;

        return newEnt;
    }

    void Scene::destroyEntity(Entity& ent) {
        if (entRegistry.valid(ent.get())) {
            entRegistry.destroy(ent.get());
            ent.nullify();
            numEnts--;
        }
        return;
    }

    Entity& Scene::getEntity(entt::entity handle) {
        auto& id1 = entRegistry.get<EntityID>(handle);
        for (auto& entItr : entities) 
            if (entRegistry.valid(entItr.second.get())) {
                auto& id2 = entRegistry.get<EntityID>(entItr.second.get());
                if (id2.ID == id1.ID) {
                    return entities[id1.ID];
                }
            }

        return entities[0];
    }

    Entity& Scene::getMainCameraEntity() {
        auto view = entRegistry.view<game::EntityID , game::CameraComponent>();
        for (auto ent : view) {
            const auto& id = view.get<game::EntityID>(ent);
            const auto& camera = view.get<game::CameraComponent>(ent);
            if (camera.camera != nullptr)
                return entities[id.ID];
        }

        return entities[0];
    }

    Entity Scene::getNullEnt() {
        return entities[0];
    }

}
}