#include "Graphics/textureAtlas.hpp"
#include "Graphics/texture.hpp"

namespace machy {
namespace graphics {

    TextureAtas::TextureAtas(std::shared_ptr<Texture> texture , int numFrames) 
        : texture(texture) , numTotalFrames(numFrames) {
            width = texture->getWidth();
            height = texture->getHeight();
    }

    

}
}