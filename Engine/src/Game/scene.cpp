#include "Game/scene.hpp"
#include "Core/callbacks.hpp"
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

    Scene::Scene() : world(nullptr) , numEnts(0) , totalEntsCreated(0) , playing(false) , name("{Blank Scene}") {

        Entity nullEnt;
        nullEnt.handle = entt::null;

        entities[0] = nullEnt;

        std::shared_ptr<graphics::VertexArray> VA = core::FileSystem::createBasicMesh();
        std::shared_ptr<graphics::VertexArray> tVA = core::FileSystem::createTexturedMesh();
        
        Meshes.load(VA->getName() , VA);
        Meshes.load(tVA->getName() , tVA);

    }

    Scene::~Scene() {

        auto view = entRegistry.view<NativeScript>();
        for (auto e : view) {
            Entity ent{ e };
            ent.setContext(this);
            ent.RemoveComponent<NativeScript>();
        }

        entRegistry.each([&] (auto entity) { entRegistry.destroy(entity); });

        if (world != nullptr) {
            delete world;
            world = nullptr;
        }

    }

    void Scene::updateFromEditor() {

        return;

    }

    void Scene::updateRuntime(const float& dt) {

        const int velIter = 8;
        const int posIter = 3;
        float timestep = (1.f / 60.f);
        world->Step(timestep , velIter , posIter);

        auto scriptView = entRegistry.view<NativeScript>();
        auto PhysicsView = entRegistry.view<PhysicsBody2DComponent>();

        for (auto ent : scriptView) {

            Entity entity{ ent };
            entity.setContext(this);
            entity.GetComponent<NativeScript>().instance->onUpdate();

        }

        for (auto e : PhysicsView) { 
            
            Entity ent{ e };
            ent.setContext(this);

            auto& pos = ent.GetComponent<PositionComponent>();
            auto& physics = ent.GetComponent<PhysicsBody2DComponent>();

            const auto& newPos = physics.runtimePhysicsBody->GetPosition();

            pos.pos = { newPos.x , newPos.y , pos.pos.z };
            pos.rotation.z = physics.runtimePhysicsBody->GetAngle();

        }

        entRegistry.view<CameraComponent , PositionComponent>().each([=](auto entity , auto& cam , auto& pos) {
                pos.pos.x = cam.cameraPos.x;
                pos.pos.y = cam.cameraPos.y;
                pos.pos.z = cam.cameraPos.z;
                pos.rotation.x = cam.cameraRotation;
                cam.camera->setHeight(pos.pos.z);
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
                        Entity entity{ ent };
                        entity.setContext(this);
                        auto& cam = entity.GetComponent<CameraComponent>();
                        MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , cam.camera));
                    }
        }

        for (const auto ent : sprites) {
            if (entRegistry.valid(ent)) {
                Entity entity{ ent };
                entity.setContext(this);
                if (entity.HasComponent<RenderComponent>() && entity.HasComponent<RenderComponent>()){
                    auto& renderable = entity.GetComponent<RenderComponent>();
                    auto& position = entity.GetComponent<PositionComponent>();

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

    void Scene::playScene() {
        playing = true;

        b2Vec2 grav{ 0.f , -9.8f };
        world = new b2World(grav);

        auto view = entRegistry.view<PhysicsBody2DComponent>();
        for (auto e : view) {
            
            Entity ent{ e };
            ent.setContext(this);

            auto& id = ent.GetComponent<EntityID>();
            auto& pos = ent.GetComponent<PositionComponent>();
            auto& physics = ent.GetComponent<PhysicsBody2DComponent>();

            physics.bodyDef.position.Set(pos.pos.x , pos.pos.y);
            physics.bodyDef.angle = pos.rotation.z;
            physics.bodyDef.type = (b2BodyType)physics.type;
            physics.bodyDef.fixedRotation = physics.fixedRotation;
            physics.bodyDef.gravityScale = 1.f;

            physics.boundBox.SetAsBox((physics.size.x * pos.size.x) , (physics.size.y * pos.size.y));

            physics.fixtrDef.shape = & (physics.boundBox);
            physics.fixtrDef.density = physics.density;
            physics.fixtrDef.friction = physics.friction;
            physics.fixtrDef.restitution = physics.restitution;
            physics.fixtrDef.restitutionThreshold = physics.restitutionThreshold;
            
            physics.runtimePhysicsBody = world->CreateBody(&physics.bodyDef);
            physics.runtimeFixture = physics.runtimePhysicsBody->CreateFixture(&physics.fixtrDef);

        }

        entRegistry.view<NativeScript>().each([=](auto entity , auto& script) {
                if (script.instance == nullptr) {
                    script.instance = script.BindScript();
                    script.instance->entity = { entity };
                    script.instance->entity.setContext(this);
                    script.instance->onCreation();
                }
            });

        return;
    }

    void Scene::pauseScene() {

        playing = false;

        if (world != nullptr) {
            delete world;
            world = nullptr;
        }

        auto view = entRegistry.view<PhysicsBody2DComponent>();
        for (auto e : view) {
            
            Entity ent{ e };
            ent.setContext(this);

            auto& physics = ent.GetComponent<PhysicsBody2DComponent>();

            physics.runtimePhysicsBody = nullptr;

        }   

        return;
    }

    void Scene::stopScene() {
        playing = false;

        auto view = entRegistry.view<NativeScript>();
        for (auto e : view) {
            Entity ent{ e };
            ent.setContext(this);
            ent.RemoveComponent<NativeScript>();
        }

        entRegistry.each([&] (auto entity) { entRegistry.destroy(entity); });

        if (world != nullptr) {
            delete world;
            world = nullptr;
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