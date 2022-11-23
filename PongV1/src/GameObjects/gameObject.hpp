#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP

#include "machy.hpp"

#include "Graphics/mesh.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/framebuffer.hpp"

#include "glm/glm.hpp"

#include <memory>

class GameObject {
    std::shared_ptr<machy::graphics::Mesh> mesh;
    std::shared_ptr<machy::graphics::Shader> shader;

    glm::vec2 pos;
    glm::vec2 size;

    public:
        GameObject(std::shared_ptr<machy::graphics::Mesh> mesh , std::shared_ptr<machy::graphics::Shader> shader ,
            const glm::vec2& pos , const glm::vec2& size) 
            : mesh(mesh) , shader(shader) , pos(pos) , size(size) {}

        void setPos(const glm::vec2& pos) { this->pos = pos; }
        void move(const glm::vec2& by) { pos += by; }

        const glm::vec2& getPos() const { return pos; }
        const glm::vec2& getSize() const { return size; }

        void Update() {}
        void Render();
};

#endif