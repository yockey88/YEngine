#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include "Graphics/renderCmnd.hpp"

#include <queue>

namespace machy {
namespace managers {

    class RenderManager {
        std::queue<std::unique_ptr<graphics::rendercommands::RenderCommand>> renderCmnds;
        public:
            RenderManager();

            void init();
            void shutdown();

            void setClearColor(float r , float g , float b , float a);

            void submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rc);
            
            // executes commands in order they were received
            void flush();

            void clear();

    };

}
}

#endif