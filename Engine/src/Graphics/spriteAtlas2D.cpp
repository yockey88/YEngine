#include "Graphics/spriteAtlas2D.hpp"

#include "Graphics/sprite2D.hpp"
#include "Graphics/material.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/vertex.hpp"

namespace machy {
namespace graphics {

    SpriteAtlas2D::SpriteAtlas2D(std::shared_ptr<Material> material) : material(material) , numTotalSprites(0) {}

    void SpriteAtlas2D::createSprite(const glm::vec2& size , const glm::vec2& posInTexr) {

        sprites[numTotalSprites] = std::make_shared<Sprite2D>(material , size , posInTexr);
        numTotalSprites++;

        return;
    }

}  
} 