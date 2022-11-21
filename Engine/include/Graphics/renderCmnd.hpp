#ifndef RENDER_COMMANDS_HPP
#define RENDER_COMMANDS_HPP

#include <memory>

namespace machy {
namespace graphics {

    class Mesh;
    class Shader;

namespace rendercommands {

    class RenderCommand {
        public:
            RenderMesh(std::weak_ptr<Mesh> mesh , std::weak_ptr<Shader> shader) : mesh(mesh) , shader(shader) {}
            virtual void execute() = 0;
    };

    class RenderMesh : public RenderCommand {
        std::weak_ptr<Mesh> mesh;
        std::weak_ptr<Shader> shader;
        public:
            RenderMesh();
    };

}
}
}

#endif