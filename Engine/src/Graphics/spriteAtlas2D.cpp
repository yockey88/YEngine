#include "log.hpp"

#include "Graphics/spriteAtlas2D.hpp"

#include "Graphics/sprite2D.hpp"
#include "Graphics/material.hpp"
#include "Graphics/texture.hpp"

namespace machy {
namespace graphics {

    SpriteAtlas2D::SpriteAtlas2D(std::shared_ptr<Material> material , std::string name) 
        : numTotalSprites(0) {
        spriteSheets[name] = material;
    }

    void SpriteAtlas2D::addSpriteSheet(std::shared_ptr<Material> material , std::string name) {
        spriteSheets[name] = material;
        return;
    }

    void SpriteAtlas2D::createSprite(std::string matName , std::string spriteName , const glm::ivec2& posCenter , const glm::ivec2& pixSize) {
        std::shared_ptr<Material> mat = spriteSheets[matName];

        int pW = pixSize.x;
        int pH = pixSize.y;

        int tW = mat->getTexture()->getWidth();
        int tH = mat->getTexture()->getHeight();

        float wRatio = (float)pW / (float)tW;
        float hRatio = (float)pH / (float)tH;

        float cxRatio = (float)posCenter.x / (float)tW;
        float cyRatio = (1.f - (float)posCenter.y) / (float)tH;

        glm::vec2 size{ wRatio , hRatio };
        glm::vec2 pos{ cxRatio , cyRatio };
        std::shared_ptr<Sprite2D> newSprite = std::make_shared<Sprite2D>(mat , size , pos , spriteName);
        newSprite->setPos({ 0.f , 0.f });
        if (pW > pH) {
            newSprite->setSize({ ((float)pW / (float)pW) , 1.f }); 
        } else {
            newSprite->setSize({ 1.f , ((float)pH / (float)pW) });
        }

        sprites[spriteName] = newSprite;
        numTotalSprites++;

        return;
    }

}  
} 