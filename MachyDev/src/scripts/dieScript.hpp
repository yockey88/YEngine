#ifndef DIE_SCRIPT_HPP
#define DIE_SCRIPT_HPP

#include "Core/callbacks.hpp"

#include "Input/keyboard.hpp"

#include "Game/Entity/entityComponents.hpp"
#include "Game/Entity/scriptedEntity.hpp"

#include "Graphics/material.hpp"

#include "imgui.h"
#include "box2d/box2d.h"

#include <vector>
#include <memory>

namespace machy {

    class DieScript : public game::ScriptedEntity {
        public:
            DieScript() {}
            virtual ~DieScript() {}

            virtual void onCreation() override {}
            
            virtual void onUpdate() override {}

    };

}

#endif