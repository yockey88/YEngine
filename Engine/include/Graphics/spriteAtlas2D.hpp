#ifndef SPRITE_ATLAS_2D_HPP
#define SPRITE_ATLAS_2D_HPP

#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

namespace machy {
namespace graphics {

    class Material;
    class Sprite2D;

    class SpriteAtlas2D {
        std::map<std::string , std::shared_ptr<Material>> spriteSheets;
        std::unordered_map<std::string , std::shared_ptr<Sprite2D>> sprites;
        int numTotalSprites;

        inline int IJtoKey(const int& i , const int& j , const int& framesPerRow) { return (j + framesPerRow * i); }
        public:
            SpriteAtlas2D(std::shared_ptr<Material> material , std::string name);

            void addSpriteSheet(std::shared_ptr<Material> material , std::string name);
            void createSprite(std::string matName , std::string spriteName , const glm::ivec2& posCenter , const glm::ivec2& pixSize);

            inline int getNumSprites() const { return numTotalSprites; }
            inline std::shared_ptr<Sprite2D> getSprite(std::string name) { return sprites[name]; }
            inline std::shared_ptr<Material> getSpriteSheet(std::string name) { return spriteSheets[name]; }
            inline const std::unordered_map<std::string , std::shared_ptr<Sprite2D>>& getAllSprites() { return sprites; }
            inline const std::map<std::string , std::shared_ptr<Material>>& getAllSpriteSheets() { return spriteSheets; }
    };  

}
}

#endif