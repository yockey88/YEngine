#ifndef STATE_HPP
#define STATE_HPP

#include "Core/window.hpp"
#include "app.hpp"
#include "log.hpp"
#include "Managers/logManager.hpp"
#include "Managers/resourceManager.hpp"
#include "Managers/renderManager.hpp"
#include "Managers/sceneManager.hpp"
#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include <iostream>

namespace machy {

    struct GlobalState {
        core::Window window;

		managers::LogManager log;
        managers::ResourceManager resources;

        managers::RenderManager renderer;
        managers::SceneManager app_scenes;

        std::string name;
        std::string version;

        GlobalState() : name("[[BLANK]]") , version("{0.0.0}]") {}

        [[nodiscard]] bool create(AppData* data , bool engineInitCheck);
    };

}



#endif /* state.hpp */