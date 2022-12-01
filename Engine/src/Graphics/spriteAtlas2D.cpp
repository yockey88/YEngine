#include "Graphics/spriteAtlas2D.hpp"

#include "Graphics/sprite2D.hpp"
#include "Graphics/material.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/vertex.hpp"

namespace machy {
namespace graphics {

    SpriteAtlas2D::SpriteAtlas2D(std::shared_ptr<Material> material) : material(material) , numTotalSprites(0) , mapLayout({ 0 , 0 }) {}

    void SpriteAtlas2D::createSprite(glm::ivec2 pixSize , glm::ivec2 posOfURInTexr) {

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

    void SpriteAtlas2D::createRandomAtlas(glm::ivec2 pixelD) {

        framePixelDim = pixelD;

        int urPosx;
        int urPosy;

        for (int i = 0; i < pixelD.y; i++) 
            for (int j = 0; j < pixelD.x; j++) {
                urPosx = j * pixelD.x; urPosy = i * pixelD.y;
                createSprite({ pixelD.x , pixelD.y } , { urPosx , urPosy });
            }

        frameDim = glm::vec2((pixelD.x / material->getTexture()->getWidth()) , (pixelD.y / material->getTexture()->getHeight()));
        atlasFormed = true;

        return;
    }

    void SpriteAtlas2D::createMap(glm::ivec2 sceneDim) {

        

        return;
    }

}  
} 