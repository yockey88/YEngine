#ifndef RENDER_COMMANDS_HPP
#define RENDER_COMMANDS_HPP

#include <memory>

namespace machy {
namespace graphics {

    class Mesh;
    class Shader;
    class Framebuffer;

namespace rendercommands {

    class RenderCommand {
        public:
            RenderCommand() {}
            virtual void execute() = 0;
    };

    class RenderMesh : public RenderCommand {
        std::weak_ptr<Mesh> mesh;
        std::weak_ptr<Shader> shader;
        public:
            RenderMesh(std::weak_ptr<Mesh> mesh , std::weak_ptr<Shader> shader) : mesh(mesh) , shader(shader) {}

            virtual void execute() override;
    };

    class PushFramebuffer: public RenderCommand {
        std::weak_ptr<Framebuffer> fBuffer;
        public:
            PushFramebuffer(std::weak_ptr<Framebuffer> buffer) : fBuffer(buffer) {}
            virtual void execute() override;
    };

     class PopFramebuffer: public RenderCommand {
        public:
            PopFramebuffer() {}
            virtual void execute() override;
    };

}
}
}

#endif