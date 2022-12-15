#ifndef CAMERA_SCRIPT_HPP
#define CAMERA_SCRIPT_HPP

#include "Game/Entity/scriptedEntity.hpp"
#include "Game/Entity/entityComponents.hpp"
#include "Input/keyboard.hpp"

namespace machy {

    class CameraScript : public game::ScriptedEntity {
        public:
            CameraScript() {}
            ~CameraScript() {}

            virtual void onCreation() override {}

            virtual void onDestruction() override {}

            virtual void onUpdate() override {
                
                auto& cam = GetComponent<game::CameraComponent>();

                if (input::keyboard::keyDown(MACHY_INPUT_KEY_W)) { cam.cameraPos.y += 0.02f; }
                if (input::keyboard::keyDown(MACHY_INPUT_KEY_A)) { cam.cameraPos.x -= 0.02f; }
                if (input::keyboard::keyDown(MACHY_INPUT_KEY_S)) { cam.cameraPos.y -= 0.02f; }
                if (input::keyboard::keyDown(MACHY_INPUT_KEY_D)) { cam.cameraPos.x += 0.02f; }

                cam.camera->setViewMat(cam.cameraPos , cam.cameraRotation);

                return;
            }
    };

}

#endif