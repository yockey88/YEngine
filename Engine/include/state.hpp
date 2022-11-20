#ifndef STATE_HPP
#define STATE_HPP

#include "Core/window.hpp"
#include "app.hpp"
#include "Managers/logManager.hpp"
#include "Managers/sceneManager.hpp"
#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"

namespace machy {

    struct GlobalState {
        core::Window window;

		managers::LogManager log;
		managers::SceneManager scenes;

        std::string name;
        std::string version;

        GlobalState() : name("[[BLANK]]") , version("{0.0.0}]") {}

        [[nodiscard]] bool create(AppData* data);
    };

}



#endif /* state.hpp */