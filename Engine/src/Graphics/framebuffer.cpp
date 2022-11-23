#include "log.hpp"
#include "Graphics/framebuffer.hpp"
#include "Graphics/helper.hpp"

#include "glad/glad.h"

namespace machy {
namespace graphics {

    Framebuffer::Framebuffer(int w , int h) : fbo(0) , textureID(0) , renderBufferID(0) , size({w , h}) , clearColor({1 , 1 , 1 , 1}) {
        
        glGenFramebuffers(1 , &fbo); MACHY_CHECK_GL_ERROR;
        glBindFramebuffer(GL_FRAMEBUFFER , fbo); MACHY_CHECK_GL_ERROR;

        // create color texture
        glGenTextures(1 , &textureID); MACHY_CHECK_GL_ERROR;
        glBindTexture(GL_TEXTURE_2D , textureID); MACHY_CHECK_GL_ERROR;
        glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , size.x , size.y , 0 , GL_RGBA , GL_UNSIGNED_BYTE , nullptr); MACHY_CHECK_GL_ERROR;
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR); MACHY_CHECK_GL_ERROR;
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR); MACHY_CHECK_GL_ERROR;
        glBindTexture(GL_TEXTURE_2D , 0); MACHY_CHECK_GL_ERROR;
        glFramebufferTexture2D(GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D , textureID , 0); MACHY_CHECK_GL_ERROR;

        // create depth/stencil renderbuffer
        glGenRenderbuffers(1 , &renderBufferID); MACHY_CHECK_GL_ERROR;
        glBindRenderbuffer(GL_RENDERBUFFER , renderBufferID); MACHY_CHECK_GL_ERROR;
        glRenderbufferStorage(GL_RENDERBUFFER , GL_DEPTH24_STENCIL8 , size.x , size.y); MACHY_CHECK_GL_ERROR;
        glBindRenderbuffer(GL_RENDERBUFFER , 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER , GL_DEPTH_STENCIL_ATTACHMENT , GL_RENDERBUFFER , renderBufferID); MACHY_CHECK_GL_ERROR;
        
        // check compeleteness
        uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER); MACHY_CHECK_GL_ERROR;
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            MACHY_ERROR("Failure to Compelete Framebuffer | Complete Status -> {}" , status);
        }
        glBindFramebuffer(GL_FRAMEBUFFER , 0); MACHY_CHECK_GL_ERROR;
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1 , &fbo);
        fbo = 0;
        textureID = 0;
        renderBufferID = 0;
    }

}
}