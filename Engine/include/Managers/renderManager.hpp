#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include "Graphics/renderCmnd.hpp"

#include "glm/glm.hpp"

#include <queue>
#include <stack>

#define MACHY_SUBMIT_RENDER_CMND(type , ...) std::move(std::make_unique<machy::graphics::rendercommands::type>(__VA_ARGS__))

namespace machy {
namespace managers {

    class RenderManager {
        friend class graphics::rendercommands::PushFramebuffer;
        friend class graphics::rendercommands::PopFramebuffer;
        friend class graphics::rendercommands::PushCamera;
        friend class graphics::rendercommands::PopCamera;

        std::queue<std::unique_ptr<graphics::rendercommands::RenderCommand>> renderCmnds;
        std::stack<std::shared_ptr<graphics::Framebuffer>> frameBuffers;
        std::stack<std::shared_ptr<graphics::Camera>> cameras;

        void pushFrameBuffer(std::shared_ptr<graphics::Framebuffer> newBuffer);
        void popFrameBuffer(); 
        void pushCamera(std::shared_ptr<graphics::Camera> newCamera);
        void popCamera();
        public:
            RenderManager();

            inline graphics::Camera* getActiveCamera() const { return (cameras.size() > 0) ? cameras.top().get() : nullptr; }

            void init();
            void shutdown();

            void setViewport(const glm::ivec4& dimensions);
            void setClearColor(const glm::vec4& clearColor);
            void setWireFrameMode(bool enabled);

            void submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rc);
            
            // executes commands in order they were received
            void flush();

            void clear();
    };

}
}

#endif