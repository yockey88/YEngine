#include "Graphics/spriteAtlas2D.hpp"

#include "Graphics/sprite2D.hpp"
#include "Graphics/material.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/vertex.hpp"

namespace machy {
namespace graphics {

    SpriteAtlas2D::SpriteAtlas2D(std::shared_ptr<Material> material) : material(material) , numTotalSprites(0) {}

    void SpriteAtlas2D::createSprite(const glm::ivec2& pixSize , const glm::ivec2& posOfURInTexr) {

        int pW = pixSize.x , pH = pixSize.y;
        int pX = posOfURInTexr.x , pY = posOfURInTexr.y;

        float wRatio =  ((float)pW / (float)material->getTexture()->getWidth());
        float hRatio =  ((float)pH / (float)material->getTexture()->getHeight());

        float cXRatio =  ((float)pX / (float)material->getTexture()->getWidth()) + (wRatio / 2.f);
        float cYRatio =  ((float)pY / (float)material->getTexture()->getHeight()) - (hRatio / 2.f); 

        glm::vec2 posInTexr{ cXRatio , cYRatio };
        glm::vec2 size{ wRatio , hRatio };

        sprites[numTotalSprites] = std::make_shared<Sprite2D>(material , size , posInTexr);
        numTotalSprites++;

        return;
    }

}  
} 