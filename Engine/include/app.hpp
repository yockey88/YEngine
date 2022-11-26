#ifndef APP_HPP
#define APP_HPP

#include "machy.hpp"
#include "Core/window.hpp"

#include <iostream>
#include <string>

namespace machy {

    class App {
        public:
            virtual ~App() {}

            virtual core::WindowProperties GetWindowProperties() { return core::WindowProperties(); }

            virtual void InitializeLibraries() {}
            virtual void Initialize() {}
            virtual void Shutdown() {}

            virtual void Update() {}
            virtual void Render() {}
            virtual void ImGuiRender() {}
    };
    
}

#endif 