#ifndef SPRITE_2D_HPP
#define SPRITE_2D_HPP

#include <memory>
#include <stdint.h>

#include "glm/glm.hpp"

namespace machy {
namespace graphics {

    class VertexArray;
    class Material;

    class Sprite2D {
        std::shared_ptr<VertexArray> VA;
        std::shared_ptr<Material> material;
        glm::vec2 gamePos , size;
        glm::vec4 bounds;
        int ID , startIndex;

        public:
            Sprite2D(std::shared_ptr<Material> material , const glm::vec2& size , const glm::vec2& posInTexr);

            void render();

            inline void setPos(const glm::vec2& pos) { gamePos = pos; }
            inline void setID(const int& ID) { this->ID = ID; }
            inline void setSize(const glm::vec2& size) { this->size = size; }

            void moveUD(const float& by) { gamePos.y += by; }
            void moveLR(const float& by) { gamePos.x += by; }

            std::shared_ptr<Material> getMat() { return material; }
            std::shared_ptr<VertexArray> getVA() { return VA; }

            inline glm::vec2 getSize() const { return size; } 
            inline int getID() const { return ID; }
    };

}
}

#endif