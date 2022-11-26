#include "log.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/helper.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad/glad.h"

#include <stdint.h>

namespace machy {
namespace graphics {

    void Texture::loadTexture() {
        glGenTextures(1 , &id); MACHY_CHECK_GL_ERROR;
        glBindTexture(GL_TEXTURE_2D , id); MACHY_CHECK_GL_ERROR;

        GLenum dataFormat = 0;
        if (numChannels == 4) {
            dataFormat = GL_RGBA;
        } else if (numChannels == 3) {
            dataFormat = GL_RGB;
        }

        if (pixels == nullptr && dataFormat == 0) {
            MACHY_ERROR("Texture File Not Found | loading default");
        } else if (pixels != nullptr && dataFormat == 0)
            MACHY_WARN("Texture Format Unsupported - numChannels -> {}" , numChannels);

        if (pixels != nullptr && dataFormat != 0) {
            glTexImage2D(GL_TEXTURE_2D , 0 , dataFormat , width , height , 0 , dataFormat , GL_UNSIGNED_BYTE , pixels); MACHY_CHECK_GL_ERROR;
            setTextFilter(filter);
            MACHY_TRACE("Loaded {} channel texture -> {}" , numChannels , path.c_str());
        } else {
            float pixels[] = 
            {   0.f , 0.f , 1.f ,    0.f , 1.f , 1.f ,   0.f , 0.f , 1.f ,   0.f , 1.f , 1.f ,
                0.f , 1.f , 1.f ,    0.f , 0.f , 1.f ,   0.f , 1.f , 1.f ,   0.f , 0.f , 1.f ,
                0.f , 0.f , 1.f ,    0.f , 1.f , 1.f ,   0.f , 0.f , 1.f ,   0.f , 1.f , 1.f , 
                0.f , 1.f , 1.f ,    0.f , 0.f , 1.f ,   0.f , 1.f , 1.f ,   0.f , 0.f , 1.f };
            width = 4;
            height = 4;
            numChannels = 3;

            glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB , width , height , 0 , GL_RGB , GL_FLOAT , pixels); MACHY_CHECK_GL_ERROR;
            setTextFilter(TextureFilter::nearest);
            MACHY_WARN("Could not load {} channel texture -> {} | default loaded" , numChannels , path.c_str());
        }
        glBindTexture(GL_TEXTURE_2D , 0);
    }

    Texture::Texture(const std::string& path) 
        : filter(TextureFilter::linear) , path(path) , width(0) , height(0) , numChannels(0) {

        int w , h , nC;
        stbi_set_flip_vertically_on_load(1);
        pixels = stbi_load(path.c_str() , &w , &h , &nC , 0);
        
        if (pixels != nullptr) {
            width = (uint32_t)w;
            height = (uint32_t)h;
            numChannels = (uint32_t)nC;
        }

        loadTexture();

    }

    Texture::~Texture() {
        stbi_image_free(pixels); 
        pixels = nullptr;
    }

    void Texture::bind() {
        glActiveTexture(GL_TEXTURE0); MACHY_CHECK_GL_ERROR;
        glBindTexture(GL_TEXTURE_2D , id); MACHY_CHECK_GL_ERROR;
        return;
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D , 0); MACHY_CHECK_GL_ERROR;
        return;
    }

    void Texture::setTextFilter(const TextureFilter& filter) {
        this->filter = filter;
        glBindTexture(GL_TEXTURE_2D , id); MACHY_CHECK_GL_ERROR;
        switch (filter) {
            case TextureFilter::linear:
                glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR); MACHY_CHECK_GL_ERROR;
                glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR); MACHY_CHECK_GL_ERROR;
            break;
            case TextureFilter::nearest:
                glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST); MACHY_CHECK_GL_ERROR;
                glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST); MACHY_CHECK_GL_ERROR;
            break;
            default:
                glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST); MACHY_CHECK_GL_ERROR;
                glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST); MACHY_CHECK_GL_ERROR;
            break;
        }
        glBindTexture(GL_TEXTURE_2D , 0); MACHY_CHECK_GL_ERROR;
        return;
    }

}
}