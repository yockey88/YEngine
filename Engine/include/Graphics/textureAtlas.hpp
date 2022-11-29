#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include <memory>
#include <stdint.h>

#include "glm/glm.hpp"

namespace machy {
namespace graphics {

    class Texture;

    class TextureAtas {
        std::shared_ptr<Texture> texture;
        int numTotalFrames;
        uint32_t width , height;

        public:
            TextureAtas(std::shared_ptr<Texture> texture , int numFrames);

            inline int getNumTotalFrames() const { return numTotalFrames; }
            inline uint32_t getWidth() const { return width; }
            inline uint32_t getHeight() const { return height; }

            /* std::shared_ptr<Animation> getAnimation(const int& firstFrame , const int& lastFrame) */

    };

}
}

#endif