#ifndef RENDER_COMMANDS_HPP
#define RENDER_COMMANDS_HPP

#include "glm/glm.hpp"

#include <memory>

namespace machy {
namespace graphics {

    class Camera;
    class VertexArray;
    class Shader;
    class Texture;
    class Framebuffer;
    class Material;

namespace rendercommands {

    class RenderCommand {
        public:
            RenderCommand() {}
            virtual void execute() = 0;
    };

    class RenderVertexArray : public RenderCommand {
        std::weak_ptr<VertexArray> vertArray;
        std::weak_ptr<Shader> shader;
        glm::mat4 modelMatrix;
        public:
            RenderVertexArray(std::weak_ptr<VertexArray> vertArray, std::weak_ptr<Shader> shader , const glm::mat4& modelMat = glm::mat4(1.f)) 
                : vertArray(vertArray) , shader(shader) , modelMatrix(modelMat) {}

            virtual void execute() override;
    };

    class RenderTexturedVertexArray : public RenderCommand {
        std::weak_ptr<VertexArray> vertArray;
        std::weak_ptr<Shader> shader;
        std::weak_ptr<Texture> texture;
        glm::mat4 modelMatrix;
        public:
            RenderTexturedVertexArray(std::weak_ptr<VertexArray> vertArray , std::weak_ptr<Shader> shader , std::weak_ptr<Texture> texture , const glm::mat4& modelMat = glm::mat4(1.f))
                : vertArray(vertArray) , shader(shader) , texture(texture) , modelMatrix(modelMat) {}

            virtual void execute() override;
    };

    class RenderVertexArrayMaterial : public RenderCommand {
        std::weak_ptr<VertexArray> vertArray;
        std::weak_ptr<Material> material;
        glm::mat4 modelMatrix;
        public:
            RenderVertexArrayMaterial(std::weak_ptr<VertexArray> vertArray , std::weak_ptr<Material> material , const glm::mat4& modelMat = glm::mat4(1.f))
                : vertArray(vertArray) , material(material) , modelMatrix(modelMat) {}

            virtual void execute() override;
    };

    class PushFramebuffer: public RenderCommand {
        std::weak_ptr<Framebuffer> fBuffer;
        public:
            PushFramebuffer(std::weak_ptr<Framebuffer> buffer)
                : fBuffer(buffer) {}
            virtual void execute() override;
    };

    class PopFramebuffer: public RenderCommand {
        public:
            PopFramebuffer() {}
            virtual void execute() override;
    };

    class PushCamera: public RenderCommand {
        std::weak_ptr<Camera> camera;
        public:
            PushCamera(std::weak_ptr<Camera> camera) : camera(camera) {}
            virtual void execute() override;
    };

    class PopCamera: public RenderCommand {
        public:
            PopCamera() {}
            virtual void execute() override;
    };

}
}
}

#endif