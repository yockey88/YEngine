#ifndef PLAYER_SCRIPT_HPP
#define PLAYER_SCRIPT_HPP

#include "Game/Entity/scriptedEntity.hpp"
#include "Game/Entity/entityComponents.hpp"
#include "Input/keyboard.hpp"

namespace machy {

    class PlayerScript : public game::ScriptedEntity {
        public:
            PlayerScript() {}
            ~PlayerScript() {}

            virtual void onCreation() override {}

            virtual void onDestruction() override {}

            virtual void onUpdate() override {
                
                auto& pos = GetComponent<game::PositionComponent>();

                if (input::keyboard::keyDown(MACHY_INPUT_KEY_A)) { pos.pos.x -= 0.2f; }
                if (input::keyboard::keyDown(MACHY_INPUT_KEY_D)) { pos.pos.x += 0.2f; }
                if (input::keyboard::keyDown(MACHY_INPUT_KEY_SPACE)) { pos.pos.y += 0.5f; }

                if (HasComponent<game::PhysicsBody2DComponent>()) {
                    auto& physics = GetComponent<game::PhysicsBody2DComponent>();

                    physics.runtimePhysicsBody->SetTransform({ pos.pos.x , pos.pos.y } , pos.rotation.z);
                } 

                return;
            }
    };

}

#endif