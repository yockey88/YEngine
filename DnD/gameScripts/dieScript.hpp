#ifndef DIE_SCRIPT_HPP
#define DIE_SCRIPT_HPP

#include "die.hpp"

#include "Core/callbacks.hpp"
#include "Core/fileSystem.hpp"

#include "Input/keyboard.hpp"

#include "Game/Entity/entityComponents.hpp"
#include "Game/Entity/scriptedEntity.hpp"

#include "Graphics/material.hpp"

#include "imgui.h"
#include "box2d/box2d.h"

#include <vector>
#include <memory>
#include <random>

namespace machy {

    class DieScript : public game::ScriptedEntity {
        std::shared_ptr<Die> die;
        
        // std::shared_ptr<graphics::VertexArray> d4_1;
        // std::shared_ptr<graphics::VertexArray> d4_2;
        // std::shared_ptr<graphics::VertexArray> d4_3;
        // std::shared_ptr<graphics::VertexArray> d4_4;

        std::shared_ptr<graphics::VertexArray> d6_1;
        std::shared_ptr<graphics::VertexArray> d6_2;
        std::shared_ptr<graphics::VertexArray> d6_3;
        std::shared_ptr<graphics::VertexArray> d6_4;
        std::shared_ptr<graphics::VertexArray> d6_5;
        std::shared_ptr<graphics::VertexArray> d6_6;

        // std::shared_ptr<graphics::VertexArray> d8_1;
        // std::shared_ptr<graphics::VertexArray> d8_2;
        // std::shared_ptr<graphics::VertexArray> d8_3;
        // std::shared_ptr<graphics::VertexArray> d8_4;
        // std::shared_ptr<graphics::VertexArray> d8_5;
        // std::shared_ptr<graphics::VertexArray> d8_6;
        // std::shared_ptr<graphics::VertexArray> d8_7;
        // std::shared_ptr<graphics::VertexArray> d8_8;

        // std::shared_ptr<graphics::VertexArray> d10_1;
        // std::shared_ptr<graphics::VertexArray> d10_2;
        // std::shared_ptr<graphics::VertexArray> d10_3;
        // std::shared_ptr<graphics::VertexArray> d10_4;
        // std::shared_ptr<graphics::VertexArray> d10_5;
        // std::shared_ptr<graphics::VertexArray> d10_6;
        // std::shared_ptr<graphics::VertexArray> d10_7;
        // std::shared_ptr<graphics::VertexArray> d10_8;
        // std::shared_ptr<graphics::VertexArray> d10_9;
        // std::shared_ptr<graphics::VertexArray> d10_10;

        // std::shared_ptr<graphics::VertexArray> d22_1;
        // std::shared_ptr<graphics::VertexArray> d22_2;
        // std::shared_ptr<graphics::VertexArray> d22_3;
        // std::shared_ptr<graphics::VertexArray> d22_4;
        // std::shared_ptr<graphics::VertexArray> d22_5;
        // std::shared_ptr<graphics::VertexArray> d22_6;
        // std::shared_ptr<graphics::VertexArray> d22_7;
        // std::shared_ptr<graphics::VertexArray> d22_8;
        // std::shared_ptr<graphics::VertexArray> d22_9;
        // std::shared_ptr<graphics::VertexArray> d22_10;
        // std::shared_ptr<graphics::VertexArray> d22_11;
        // std::shared_ptr<graphics::VertexArray> d22_12;
        // std::shared_ptr<graphics::VertexArray> d20_13;
        // std::shared_ptr<graphics::VertexArray> d20_14;
        // std::shared_ptr<graphics::VertexArray> d20_15;
        // std::shared_ptr<graphics::VertexArray> d20_16;
        // std::shared_ptr<graphics::VertexArray> d20_17;
        // std::shared_ptr<graphics::VertexArray> d20_18;
        // std::shared_ptr<graphics::VertexArray> d20_19;
        // std::shared_ptr<graphics::VertexArray> d20_20;

        float currVertVelocity;
        float lastVertVelocity;

        public:
            DieScript() {}
            virtual ~DieScript() {}

            virtual void onCreation() override {
                srand((int)time(0));
                die = D(6);

                d6_1 = core::FileSystem::createTexturedMesh({ 0.5f , 1.f   , 0.5f , 0.66f  } , { 0.f  , 0.66f , 0.f  , 1.f   });
                d6_2 = core::FileSystem::createTexturedMesh({ 1.f  , 1.f   , 1.f  , 0.66f  } , { 0.5f , 0.66f , 0.5  , 1.f   });
                d6_3 = core::FileSystem::createTexturedMesh({ 0.5f , 0.66f , 0.5f , 0.33f  } , { 0.f  , 0.33f , 0.f  , 0.66f });
                d6_4 = core::FileSystem::createTexturedMesh({ 1.f   , 0.66f , 1.f  , 0.33f } , { 0.5f , 0.33f , 0.5f , 0.66f });
                d6_5 = core::FileSystem::createTexturedMesh({ 0.5f , 0.33f , 0.5f , 0.f    } , { 0.f  , 0.f   , 0.f  , 0.33f });
                d6_6 = core::FileSystem::createTexturedMesh({ 1.f  , 0.33f , 1.f  , 0.f    } , { 0.5f , 0.f   , 0.5f , 0.33f });
            }
            
            virtual void onUpdate() override {
                
                auto& pos = GetComponent<game::PositionComponent>();
                auto& sprite = GetComponent<game::RenderComponent>();
                auto& physics = GetComponent<game::PhysicsBody2DComponent>();

                currVertVelocity = physics.runtimePhysicsBody->GetLinearVelocity().y; 
                
                if (lastVertVelocity < 0 && currVertVelocity > 0) {
                    int randNum = rand() % 6;
                    switch (randNum) {
                        case 1: sprite.skeleton = d6_1; break;
                        case 2: sprite.skeleton = d6_2; break;
                        case 3: sprite.skeleton = d6_3; break;
                        case 4: sprite.skeleton = d6_4; break;
                        case 5: sprite.skeleton = d6_5; break;
                        case 0: sprite.skeleton = d6_6; break;
                        default: sprite.skeleton = d6_1; break;
                    }
                }

                lastVertVelocity = currVertVelocity;

            }

    };

}

#endif