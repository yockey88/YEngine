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
        glm::vec2 frameDim;
        glm::ivec2 framePixelDim , mapLayout;
        int numTotalSprites;
        bool atlasFormed;

        inline int IJtoKey(const int& i , const int& j , const int& framesPerRow) { return (j + framesPerRow * i); }
        public:
            SpriteAtlas2D(std::shared_ptr<Material> material);

            void createSprite(glm::ivec2 size , glm::ivec2 posOfURInTexr);
            void createRandomAtlas(glm::ivec2 dimensions);
            void createMap(glm::ivec2 mapD);

            inline int getNumSprites() const { return numTotalSprites; }
            std::shared_ptr<Sprite2D> getSprite(const int& id) { return sprites[id]; }
            const std::map<int, std::shared_ptr<Sprite2D>>& getAllSprites() { return sprites; }
    };  

}
}

#endif