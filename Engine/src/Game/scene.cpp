#include "Game/scene.hpp"
#include "Game/Entity/scriptedEntity.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "machy.hpp"

namespace machy {
namespace game {

    Scene::Scene() : numEnts(0) , name("{Blank Scene}")  {
        Entity nullEnt;
        nullEnt.handle = entt::null;

        entities[0] = nullEnt;

        {
            std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();
            MACHY_CREATE_VERTEX_BUFFER(vb , float);
            vb->pushVertex({  0.5f ,  0.5f , 0.f });
            vb->pushVertex({  0.5f , -0.5f , 0.f });
            vb->pushVertex({ -0.5f , -0.5f , 0.f });
            vb->pushVertex({ -0.5f ,  0.5f , 0.f });
            vb->setLayout({ 3 });
            VA->pushBuffer(std::move(vb));
            VA->setElements({ 0 , 3 , 1 , 1 , 3 , 2 });
            VA->upload();
            MeshLib.load("Skeleton" , VA);
        }
        { 
            std::string vShader = util::readShaderFile("resources/shaders/basic_camera_shader.vert");
            std::string fShader = util::readShaderFile("resources/shaders/basic_camera_shader.frag");
            std::string cShader = util::readShaderFile("resources/shaders/basic_circle.frag");
            std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShader , fShader);
            std::shared_ptr<graphics::Shader> circle = std::make_shared<graphics::Shader>(vShader , cShader);
            ShaderLib.load("Basic" , shader);
            ShaderLib.load("Circle" , circle);
        }
        {
            std::shared_ptr<graphics::Material> basic = std::make_shared<graphics::Material>(ShaderLib.get("Basic"));
            std::shared_ptr<graphics::Material> circle = std::make_shared<graphics::Material>(ShaderLib.get("Circle"));
            MatLib.load("Basic" , basic);
            MatLib.load("Circle" , circle);
        }
    }

    Scene::~Scene() {
        entRegistry.each([&] (auto entity) { entRegistry.destroy(entity); });
    }

    void Scene::update() {
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
        auto cameras = entRegistry.view<CameraComponent>();
        auto sprites = entRegistry.view<RenderComponent , PositionComponent>();

        if (cameras.size() > 0)
            for (auto& ent : cameras)
                if (entRegistry.valid(ent)) {
                    auto& cam = cameras.get<CameraComponent>(ent);
                    MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , cam.camera));
                }

        for (const auto ent : sprites) {
            if (entRegistry.valid(ent)) {
                auto& renderable = sprites.get<RenderComponent>(ent);
                auto& position = sprites.get<PositionComponent>(ent);

                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model , position.pos);
                model = glm::scale(model , position.size);
                if (renderable.skeleton.get() != nullptr && renderable.material.get() != nullptr)
                    MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , renderable.skeleton , renderable.material , model));
            }
        }

        for (unsigned long i = 0; i < cameras.size(); i++) {
            MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));
        }

        return;
    }

    Entity Scene::createEnt(const std::string& name) {
        Entity newEnt(entRegistry.create());
        newEnt.setContext(this);

        auto& id = entRegistry.emplace<EntityID>(newEnt.get());
        numEnts++;
        id.ID = numEnts;
        id.name = name;

        entities[id.ID] = newEnt;

        return newEnt;
    }

    // Entity createNullEnt() {
    //     Entity ent(entt::null);
    //     return ent;
    // }

    void Scene::destroyEntity(Entity& ent) {
        entRegistry.destroy(ent.get());
        numEnts--;
        return;
    }

    Entity& Scene::getEntity(entt::entity handle) {
        auto& id1 = entRegistry.get<EntityID>(handle);
        for (auto& entItr : entities) 
            if (entRegistry.valid(entItr.second.get())) {
                auto& id2 = entRegistry.get<EntityID>(entItr.second.get());
                if (id2.ID == id1.ID) {
                    return entities[id2.ID];
                }
            }

        return entities[0];
    }

    Entity Scene::getNullEnt() {
        return entities[0];
    }

}
}