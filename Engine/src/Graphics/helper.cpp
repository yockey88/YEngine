#include "Graphics/helper.hpp"

namespace machy {
namespace graphics{

    void checkGLError()  {
        GLenum error = glGetError();
        bool assert = error != GL_NO_ERROR;
        while (error != GL_NO_ERROR) {
            std::string errstr;
            switch (error) {
                case GL_INVALID_OPERATION: errstr = "INVALID OPERATION"; break;
                case GL_INVALID_ENUM: errstr = "INVALID ENUM"; break;
                case GL_INVALID_VALUE: errstr = "INVALID VALUE"; break;
                case GL_OUT_OF_MEMORY: errstr = "OUT OF MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: errstr = "INVALID FRAMEBUFFER OPERATION"; break;
                default: errstr = std::to_string(error); break;
            }

            MACHY_ERROR("OpenGL Error -> {}" , errstr.c_str());
            error = glGetError();
        }
        MACHY_ASSERT(!assert , "OpenGl Failure");

        return;
    }

}
}