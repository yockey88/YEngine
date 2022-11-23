#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include "glm/glm.hpp"

#include <stdint.h>

namespace machy {
namespace graphics {

    class Framebuffer {
        uint32_t fbo;
        uint32_t textureID;
        uint32_t renderBufferID;

        glm::ivec2 size;
        glm::vec4 clearColor;
        public:
            Framebuffer(int width , int height);
            ~Framebuffer();

            inline uint32_t getFbo() const { return fbo; }
            inline uint32_t getTextureID() const { return textureID; }
            inline uint32_t getRenderBufferID() const { return renderBufferID; }
            inline glm::ivec2& getSize() { return size; }
            void setClearColor(const glm::vec4& cc) { clearColor = cc; }
            inline glm::vec4& getClearColor() { return clearColor; } 
    };

}
}

#endif