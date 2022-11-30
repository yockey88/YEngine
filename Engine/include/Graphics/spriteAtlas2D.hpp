#ifndef SPRITE_ATLAS_2D_HPP
#define SPRITE_ATLAS_2D_HPP

#include "glm/glm.hpp"

#include <map>
#include <memory>

namespace machy {
namespace graphics {

    class VertexArray;
    class Material;
    class Sprite2D;

    class SpriteAtlas2D {
        std::map<int , std::shared_ptr<Sprite2D>> sprites;
        std::shared_ptr<Material> material;
        std::shared_ptr<VertexArray> VA;
        int numTotalSprites;

        inline int IJtoKey(const int& i , const int& j , const int& framesPerRow) { return (j + framesPerRow * i); }
        public:
            SpriteAtlas2D(std::shared_ptr<Material> material);

            void createSprite(const glm::vec2& size , const glm::vec2& posInTexr);

            inline int getNumSprites() const { return numTotalSprites; }

            const std::map<int, std::shared_ptr<Sprite2D>>& getAllSprites() { return sprites; }

            std::shared_ptr<Sprite2D> getSprite(const int& id) { return sprites[id]; }
    };  

}
}

#endif