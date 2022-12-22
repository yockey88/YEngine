#include <machy.hpp>
#include <app.hpp>
#include <main.hpp>

#include "die.hpp"
#include "dieScript.hpp"

#include "Core/fileSystem.hpp"

#include "Game/scene.hpp"
#include "Game/Entity/entity.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/camera.hpp"

#include "box2d/box2d.h"

#include <vector>
#include <memory>
#include <variant>

static std::string sceneFile = "resources/scenes/dnd.json";
static std::string matFile = "resources/assets/materials/basic.json"; 

namespace machy {

    class DnD : public App {
        std::shared_ptr<graphics::Camera> camera;

        std::shared_ptr<game::Scene> mainScene;
        std::shared_ptr<Die> die;

        glm::vec3 cameraPos;
        float cameraRot;  

        int outcome;
        bool printOutcome;

        core::WindowProperties setWindowProps();

        void createFloor(game::Entity& ent);
        void createDieEntt(game::Entity& ent);
        public:
            DnD() : camera(std::shared_ptr<graphics::Camera>( nullptr )) , mainScene(std::shared_ptr<game::Scene>( nullptr )) , 
                cameraPos({ 0.f , 0.f , 0.f }) , cameraRot(0.f), outcome(-1) , printOutcome(false) {}
            virtual ~DnD() {}

            virtual core::WindowProperties GetWindowProperties() { return setWindowProps(); }
            
            virtual void Initialize() {
                mainScene = core::FileSystem::loadScene(sceneFile);

                camera = std::make_shared<graphics::Camera>();
                cameraPos.x = 0.f;
                cameraPos.y = 0.f;
                cameraPos.z = 11.f;
                cameraRot = 0.f;
                camera->setHeight(cameraPos.z);

                die = D(20);

                auto view = mainScene->Entts().view<game::EntityID>();
                for (auto e : view) {
                    game::Entity ent{ e };
                    ent.setContext(mainScene.get());
                    if (ent.GetComponent<game::EntityID>().name == "die") {
                        ent.AddComponent<game::NativeScript>().Bind<DieScript>();
                    }
                }

            }

            virtual void Shutdown() override {}

            virtual void Update(const float& dt) override {

                
                if (input::keyboard::key(MACHY_INPUT_KEY_P) && mainScene->isPlaying()) { mainScene->pauseScene(); }
                if (input::keyboard::key(MACHY_INPUT_KEY_P) && !mainScene->isPlaying()) { mainScene->playScene(); }
                if (input::keyboard::key(MACHY_INPUT_KEY_R)) {
                    mainScene->stopScene();
                    mainScene = core::FileSystem::loadScene(sceneFile);
                    mainScene->pauseScene();

                    auto view = mainScene->Entts().view<game::EntityID>();
                    for (auto e : view) {
                        game::Entity ent{ e };
                        ent.setContext(mainScene.get());
                        if (ent.GetComponent<game::EntityID>().name == "die") {
                            ent.AddComponent<game::NativeScript>().Bind<DieScript>();
                        }
                    }

                }

                if (mainScene->isPlaying()) {
                    mainScene->updateRuntime(dt);
                }
       
            }

            virtual void Render() override {
                MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , camera));
            
                mainScene->render();

                MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));
            }

            virtual void ImGuiRender() override {

                if (ImGui::Begin("Dice Options")) {

                }
                ImGui::End();

                if (ImGui::Begin("Outcome")) {

                

                }
                ImGui::End();

            }

            void setOutcome(const int& outcome) { this->outcome = outcome; }
    };

    core::WindowProperties DnD::setWindowProps() {
        core::WindowProperties props;

        props.guiProps.isViewportEnabled = true;

        props.w = 1026;
        props.h = 576;

        props.flags |= SDL_WINDOW_RESIZABLE;
        props.title = "[DnD Dice Roller]";
        
        return props;
    }

}

machy::App* CreateApp() { return new machy::DnD; }